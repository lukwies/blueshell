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

#### Check if bluetoothd is running
```console
$ systemctl is-enabled bluetooth
$ systemctl status bluetooth
```

#### Check if bluetooth is enabled
<pre>
$ rfkill
$ sudo rfkill unblock bluetooth
</pre>

#### Start bluetoothctl
<pre>
$ bluetoothctl
</pre>

#### Turn on bluetooth adapter
<pre>
[bluetooth]# default-agent
[bluetooth]# power on
[bluetooth]# discoverable on
[bluetooth]# advertise on
</pre>

#### Scan to find device to pair with
<pre>
[bluetooth]# scan on
</pre>

#### Pair and trust device
<pre>
[bluetooth]# pair 00:11:22:33:44:55
[bluetooth]# trust 00:11:22:33:44:55
</pre>

#### Show paired devices
<pre>
[bluetooth]# paired-devices
[bluetooth]# devices
</pre>
