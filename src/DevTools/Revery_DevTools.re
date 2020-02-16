module Log = (val Revery_Core.Log.withNamespace("Revery.DevTools"));

open Lwt.Infix;

let connection_handler = {
  module Body = Httpaf.Body;
  module Headers = Httpaf.Headers;
  module Reqd = Httpaf.Reqd;
  module Response = Httpaf.Response;
  module Status = Httpaf.Status;

  let websocket_handler = (_client_address, wsd) => {
    let frame = (~opcode, ~is_fin as _, bs, ~off, ~len) => {
      switch (opcode) {
      | `Binary =>
        Websocketaf.Wsd.schedule(wsd, bs, ~kind=`Binary, ~off, ~len)
      | `Continuation =>
        Websocketaf.Wsd.schedule(wsd, bs, ~kind=`Continuation, ~off, ~len)
      | `Text => Websocketaf.Wsd.schedule(wsd, bs, ~kind=`Text, ~off, ~len)
      | `Connection_close => Websocketaf.Wsd.close(wsd)
      | `Ping => Websocketaf.Wsd.send_pong(wsd)
      | `Pong
      | `Other(_) => ()
      };

      Log.debug(
        "Websocket received opcode: "
        ++ (
          switch (opcode) {
          | `Binary => "Binary"
          | `Continuation => "Continuation"
          | `Text => "Text"
          | `Connection_close => "Connection close"
          | _ => "Other"
          }
        ),
      );
    };

    let eof = () => {
      Log.info("Client disconnected from socket.");
      Websocketaf.Wsd.close(wsd);
    };

    {Websocketaf.Server_connection.frame, eof};
  };

  let error_handler = (wsd, `Exn(exn)) => {
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

  let http_error_handler = (_client_address, ~request as _=?, error, handle) => {
    let message =
      switch (error) {
      | `Exn(exn) => Printexc.to_string(exn)
      | (#Status.client_error | #Status.server_error) as error =>
        Status.to_string(error)
      };

    let body = handle(Headers.empty);
    Body.write_string(body, message);
    Body.close_writer(body);
  };

  let upgrade_handler = (addr, socket) =>
    Websocketaf_lwt_unix.Server.create_upgraded_connection_handler(
      ~error_handler,
      ~websocket_handler,
      addr,
      socket,
    );

  let request_handler = (addr, reqd) =>
    Websocketaf_lwt_unix.Server.respond_with_upgrade(
      reqd,
      upgrade_handler(addr),
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

          Reqd.respond_with_string(reqd, response, err_str);
        }
    )
    |> Lwt.ignore_result;

  Httpaf_lwt_unix.Server.create_connection_handler(
    ~config=?None,
    ~request_handler,
    ~error_handler=http_error_handler,
  );
};

let run = () => {
  open Lwt.Infix;

  let port = ref(8080);

  let listen_address =
    Unix.([@implicit_arity] ADDR_INET(inet_addr_loopback, port^));

  Lwt.async(() =>
    Lwt_io.establish_server_with_client_socket(
      listen_address,
      connection_handler,
    )
    >>= (
      _server => {
        Log.info("Listening on port " ++ string_of_int(port^));
        Lwt.return_unit;
      }
    )
  );
};

let start = () => run();
