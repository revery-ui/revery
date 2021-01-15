open Revery;
open Revery.UI;
open Revery.UI.Components;
open Revery.Native;

module AudioHelpers = {
  let getBitsPerSample =
    fun
    | Sdl2.Audio.Format.U8
    | S8 => 8
    | S16LSB
    | S16MSB
    | U16LSB
    | U16MSB => 16
    | S32LSB
    | S32MSB
    | F32LSB
    | F32MSB => 32;

  let getAudioDuration = (spec: Sdl2.Audio.Spec.t, bytes) => {
    bytes / (spec.freq * spec.channels * (getBitsPerSample(spec.format) / 8));
  };
};

module Styles = {
  let container =
    Style.[
      position(`Absolute),
      top(0),
      bottom(0),
      left(0),
      right(0),
      alignItems(`Center),
      justifyContent(`Center),
      flexDirection(`Column),
    ];
  let text = Style.[color(Colors.white), margin(14)];
  let audioPlayerTrack =
    Style.[
      width(200),
      marginBottom(14),
      padding(2),
      borderRadius(8.),
      backgroundColor(Colors.dimGrey),
      alignItems(`Center),
      flexDirection(`Row),
      justifyContent(`Center),
    ];
};

module WavFilePlaybackExamples = {
  let%component make = () => {
    let%hook (filename, setFilename) = Hooks.state("");
    let%hook (audioData, setAudioData) = Hooks.state(None);
    let%hook (audioDevice, setAudioDevice) = Hooks.state(None);
    let%hook (audioDeviceStatus, setAudioDeviceStatus) =
      Hooks.state(Sdl2.Audio.Device.Status.Stopped);
    let%hook (audioQueuedBytes, setAudioQueuedBytes) = Hooks.state(0);
    let%hook () =
      Hooks.tick(~name="", ~tickRate=Time.milliseconds(50), _ => {
        switch (audioDevice) {
        | Some(device) =>
          let status = Sdl2.Audio.Device.getStatus(device);
          setAudioDeviceStatus(_ => status);
          let bytes = Sdl2.Audio.getQueuedSize(device);
          setAudioQueuedBytes(_ => bytes);
        | _ => ()
        }
      });

    let audioQueuedBytes = float(audioQueuedBytes);
    let audioTotalBytes =
      switch (audioData) {
      | Some((_, _, len)) => float(len)
      | _ => 0.
      };
    let audioPlayedBytes =
      audioQueuedBytes > 0. ? audioTotalBytes -. audioQueuedBytes : 0.;
    let audioPlaybackProgress = audioPlayedBytes /. audioTotalBytes;
    let audioDuration =
      switch (audioData) {
      | Some((spec: Sdl2.Audio.Spec.t, _, bytes)) =>
        AudioHelpers.getAudioDuration(spec, bytes)
      | _ => 0
      };
    let audioElapsed =
      int_of_float(float(audioDuration) *. audioPlaybackProgress);

    let openAudioDevice = desiredSpec => {
      switch (audioDevice) {
      | Some(device) => Sdl2.Audio.Device.close(device)
      | _ => ()
      };
      switch (
        Sdl2.Audio.Device.open_(
          None,
          false,
          desiredSpec,
          Sdl2.Audio.Device.AllowedChanges.none,
        )
      ) {
      | Error(err) => Console.error(err)
      | Ok((device, _obstainedSpec)) => setAudioDevice(_ => Some(device))
      };
    };

    let loadWavFile = filename => {
      switch (Sdl2.Audio.Wav.load(filename)) {
      | Error(err) => Console.error(err)
      | Ok(data) =>
        setAudioData(_ => Some(data));
        let (spec, _, _) = data;
        openAudioDevice(spec);
      };
    };

    let openFile = () => {
      let files =
        Dialog.openFiles(
          ~fileTypes=[|"wav", "wave"|],
          ~allowMultiple=false,
          ~canChooseDirectories=false,
          ~showHidden=false,
          ~title="Revery Open File Example",
          ~buttonText="Open file in Revery",
          (),
        );
      switch (files) {
      | Some([|path|]) =>
        setFilename(_ => path |> Filename.basename);
        loadWavFile(path);
      | _ => ()
      };
    };

    let play = () => {
      switch (audioData, audioDevice) {
      | (Some((spec, buf, len)), Some(device)) =>
        if (audioQueuedBytes > 0.) {
          Sdl2.Audio.Device.pause(device, false);
        } else {
          switch (Sdl2.Audio.queue(device, buf, len)) {
          | Error(err) => Console.error(err)
          | Ok(_) => Sdl2.Audio.Device.pause(device, false)
          };
        }
      | _ => ()
      };
    };

    let pause = () => {
      switch (audioDevice) {
      | Some(device) => Sdl2.Audio.Device.pause(device, true)
      | _ => ()
      };
    };

    if (filename == "") {
      <View style=Styles.container>
        <Text style=Styles.text fontSize=20. text="WAV File Playback" />
        <Button
          title="Open File"
          fontSize=12.
          height=50
          width=100
          onClick=openFile
        />
      </View>;
    } else {
      <View style=Styles.container>
        <Row>
          <Text style=Styles.text fontSize=20. text="Audio Loaded" />
        </Row>
        <Row> <Text fontSize=12. style=Styles.text text=filename /> </Row>
        <Row>
          <View style=Styles.audioPlayerTrack>
            <Text
              fontSize=12.
              style=Styles.text
              text=Time.(seconds(audioElapsed) |> toString)
            />
            <Slider
              value=audioPlayedBytes
              minimumValue=0.
              maximumValue=audioTotalBytes
              thumbThickness=0
              thumbLength=0
              thumbColor={Color.hex("#90f7ff")}
            />
            <Text
              fontSize=12.
              style=Styles.text
              text=Time.(seconds(audioDuration) |> toString)
            />
          </View>
        </Row>
        <Row>
          {switch (audioDeviceStatus) {
           | Stopped
           | Paused =>
             <Button
               title="Play"
               fontSize=12.
               height=50
               width=100
               onClick=play
             />
           | Playing =>
             audioPlaybackProgress == 1.
               ? <Button
                   title="Play"
                   fontSize=12.
                   height=50
                   width=100
                   onClick=play
                 />
               : <Button
                   title="Pause"
                   fontSize=12.
                   height=50
                   width=100
                   onClick=pause
                 />
           }}
          <Button
            title="Open File"
            fontSize=12.
            height=50
            width=100
            onClick=openFile
          />
        </Row>
      </View>;
    };
  };
};

let render = () => <WavFilePlaybackExamples />;
