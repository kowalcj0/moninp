moninp
------
This tool maps Linux Input Device events into `mpc` commands that allow
you to control [mpd](https://www.musicpd.org/).

The whole code except the Key Press/Command mapping was written by [Ronium](http://unix.stackexchange.com/users/28489/runium)
You can find it on [Unix StackExchange](http://unix.stackexchange.com/a/94329)


I use this little program to control [Volumio](https://volumio.org/) with
[Xbox One Media Remote](http://www.xbox.com/en-GB/xbox-one/accessories/controllers/media-remote)
and [1st gen Flirc USB](https://flirc.tv/more/flirc-usb-v1)
![Flirc USB 1st gen](flirc-usb-1st-gen.png)


# Provided mapping

This project uses my Flirc button mapping for Xbox One Media Remote which you 
can find in [xbox_one_media_remote_flirc_config.fcfg](https://github.com/kowalcj0/moninp/raw/master/xbox_one_media_remote_flirc_config.fcfg)

![Xbox One Media Remote Mapping](xboxonemapping.png)

# Compile

You can build the binary with your own mappings by simply compiling the source
file with `gcc`.
```bash
gcc moninp.c -o monimp
```

PS. Volumio doesn't have `gcc` installed, so you can compile it on e.g.:
(Rasbian Jessie Lite](https://www.raspberrypi.org/downloads/raspbian/)


# Run:

```bash
sudo ./moninp /dev/input/event2
```

You can also provide device ID.
My [1st gen Flirc USB](https://flirc.tv/more/flirc-usb-v1) is identified as 
`/dev/input/by-id/usb-flirc.tv_flirc-event-kbd`. So you can start `moninp`
```bash
sudo ./moninp /dev/input/by-id/usb-flirc.tv_flirc-event-kbd
```

# Using your own mapping

If you'd like to change the mapping, then ATM you will have to:

1. run `monimp` binary for your selected input device
2. press a button on your remote
3. find the button `code` for an event with `type=00` & `value=00` (key up)

e.g.: in the example below we're after `code` from line number `5` (2nd would work too :) )
```bash
1491342238.860640: type=04 code=04 value=c00cd
1491342238.860640: type=01 code=a4 value=01
1491342238.860640: type=00 code=00 value=00
1491342239.020638: type=04 code=04 value=c00cd
1491342239.020638: type=01 code=a4 value=00
1491342239.020638: type=00 code=00 value=00
1491342244.333333: type=04 code=04 value=c00b3
1491342244.333333: type=01 code=d0 value=01
```
4. add new mapping for this `code` or replace existing one
5. compile, upload binary to your `mpd` powered player and enjoy

# Futurue work

* load mappings from a simple properties file
* autodetect Flirc device

