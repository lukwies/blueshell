# blueshell

A python written shell over bluetooth.


## Usage
<pre>
 Usage: blueshell [-vh] [--listen]
                        [--connect=MACADDR]
                        [--port=PORT]

 -h, --help   Show helptext and quit
     --version  Show blueshell version

 -c, --connect=MACADDR    Connect to given mac address (client)
 -l, --listen             Bind to default device (server)
     --listen-at=MACADDR  Bind to given mac address (server)

 -s, --scan               Scan for bluetooth devices
</pre>


## Prepare devices
Before using the blueshell you need to pair server and client.
Let's check if the bluetooth daemon is running and bluetooth is enabled.
```console
$ systemctl is-enabled bluetooth
$ systemctl status bluetooth

$ rfkill
$ sudo rfkill unblock bluetooth
```

Okay, now let's start the interactive bluetoothctl shell and change some settings.
```console
$ bluetoothctl

[bluetooth]# default-agent
[bluetooth]# power on
[bluetooth]# discoverable on
[bluetooth]# advertise on
```

If we don't know the peers mac address yet, scan for nearby bluetooth devices.
```console
[bluetooth]# scan on

...

[bluetooth]# scan off
```

Let's pair with the peer device and trust remote device for further connections.
```console
[bluetooth]# pair 00:11:22:33:44:55
[bluetooth]# trust 00:11:22:33:44:55
```

Show all paired devices, the new one should occur within the output list.
```console
[bluetooth]# paired-devices
[bluetooth]# devices
```
