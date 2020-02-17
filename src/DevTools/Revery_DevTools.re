module Log = (val Revery_Core.Log.withNamespace("Revery.DevTools"));

open Lwt.Infix;
open Httpaf;

let socketAddrToString = sockaddr =>
  switch (sockaddr) {
  | Unix.ADDR_UNIX(s) => s
  | Unix.ADDR_INET(addr, port) =>
    Unix.string_of_inet_addr(addr) ++ ":" ++ string_of_int(port)
  };

let connectionHandler = (win: Revery_Core.Window.t) => {
  module Body = Httpaf.Body;
  module Headers = Httpaf.Headers;
  module Reqd = Httpaf.Reqd;
  module Response = Httpaf.Response;
  module Status = Httpaf.Status;

  let websocketHandler = (clientAddress, wsd) => {
    let frame = (~opcode, ~is_fin as _, bs, ~off, ~len) => {
      switch (opcode) {
      | `Binary =>
        Websocketaf.Wsd.schedule(wsd, bs, ~kind=`Binary, ~off, ~len)
      | `Continuation =>
        Websocketaf.Wsd.schedule(wsd, bs, ~kind=`Continuation, ~off, ~len)
      | `Text =>
        Websocketaf.Wsd.send_bytes(
          wsd,
          Bytes.of_string("ABC"),
          ~kind=`Text,
          ~off=0,
          ~len=3,
        )
      | `Connection_close => Websocketaf.Wsd.close(wsd)
      | `Ping => Websocketaf.Wsd.send_pong(wsd)
      | `Pong
      | `Other(_) => ()
      };

      Log.debug(
        "Websocket received opcode \'"
        ++ (
          switch (opcode) {
          | `Binary => "Binary"
          | `Continuation => "Continuation"
          | `Text => "Text"
          | `Connection_close => "Connection close"
          | _ => "Other"
          }
        )
        ++ "\' from address "
        ++ socketAddrToString(clientAddress),
      );

      Log.debug(
        "Websocket received string: " ++ Bigstringaf.substring(~off, ~len, bs),
      );
    };

    let eof = () => {
      Log.info("Client disconnected from socket.");
      Websocketaf.Wsd.close(wsd);
    };

    {Websocketaf.Server_connection.frame, eof};
  };

  let errorHandler = (wsd, `Exn(exn)) => {
    let message = Printexc.to_string(exn);
    let payload = Bytes.of_string(message);
    Websocketaf.Wsd.send_bytes(
      wsd,
      ~kind=`Text,
      payload,
      ~off=0,
      ~len=Bytes.length(payload),
    );
    Websocketaf.Wsd.close(wsd);
  };

  let httpErrorHandler = (_client_address, ~request as _=?, error, handle) => {
    let message =
      switch (error) {
      | `Exn(exn) => Printexc.to_string(exn)
      | (#Status.client_error | #Status.server_error) as error =>
        Status.to_string(error)
      };

    Log.debug("HTTP error: " ++ message);

    let body = handle(Headers.empty);
    Body.write_string(body, message);
    Body.close_writer(body);
  };

  let upgradeHandler = (addr, socket) =>
    Websocketaf_lwt_unix.Server.create_upgraded_connection_handler(
      ~error_handler=errorHandler,
      ~websocket_handler=websocketHandler,
      addr,
      socket,
    );

  let requestHandler = (addr, reqd) => {
    let request = Reqd.request(reqd);
    let path = Uri.of_string(request.target) |> Uri.path;

    Log.info(Method.to_string(request.meth) ++ " request on path: " ++ path);

    switch (request) {
    | {meth: `GET, _} =>
      switch (path) {
      | "/ws/revery" =>
        Websocketaf_lwt_unix.Server.respond_with_upgrade(
          reqd,
          upgradeHandler(addr),
        )
        >|= (
          fun
          | Ok () => ()
          | Error(err_str) => {
              let response =
                Response.create(
                  ~headers=Httpaf.Headers.of_list([("Connection", "close")]),
                  `Bad_request,
                );

              Log.info("Request error: " ++ err_str);

              Reqd.respond_with_string(reqd, response, err_str);
            }
        )
        |> Lwt.ignore_result
      | "/json" =>
        Log.info("Pushing initial JSON info to debugger.");
        let responseBody = "[
          {
            \"id\": \"revery\",
            \"title\": \"Revery UI\"
          }
        ]";
        let headers =
          Headers.of_list([
            ("Content-Length", string_of_int(String.length(responseBody))),
            ("Content-Type", "application/json"),
          ]);
        Reqd.respond_with_string(
          reqd,
          Response.create(~headers, `OK),
          responseBody,
        );
      | _ =>
        let responseBody = "
          <html>
            <body>
              <h1>404: Resource not found</h1>
              <p>Revery DevTools<p>
            </body>
          </html>";
        let headers =
          Headers.of_list([
            ("Content-Length", string_of_int(String.length(responseBody))),
          ]);
        Reqd.respond_with_string(
          reqd,
          Response.create(~headers, `Not_found),
          responseBody,
        );
      }
    | _ => ()
    };
  };

  Httpaf_lwt_unix.Server.create_connection_handler(
    ~config=?None,
    ~request_handler=requestHandler,
    ~error_handler=httpErrorHandler,
  );
};

let start = win => {
  open Lwt.Infix;

  let port = ref(8080);

  let listenAddr =
    Unix.([@implicit_arity] ADDR_INET(inet_addr_loopback, port^));

  Lwt.async(() =>
    Lwt_io.establish_server_with_client_socket(
      listenAddr,
      connectionHandler(win),
    )
    >>= (
      _server => {
        Log.info("Listening on port " ++ string_of_int(port^));
        Lwt.return_unit;
      }
    )
  );
};
