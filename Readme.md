# blueshell
# upcoming next ...
blueshell is a program for logging into a remote machine via bluetooth to execute shell commands.<br>
Before a connection can be established a blueshell server need to run on the machine you want to control.<br>



## Usage
<pre>
Usage: blueshell [OPTIONS] ...

-h, --help               Show helptext and quit
    --version            Show blueshell version

-a, --addr=MAC(/PORT)    Set macaddress (and port)
-c, --client             Run in client mode
-s, --scan               Scan for bluetooth devices

-v, --verbose
-q, --quiet

</pre>

## Interactive client mode
When running blueshell in client mode an interactive shell will appear.<br>
There are three types of commands:
1. Blueshell internal commands
2. Commands executed at the server
3. Commands executed at the client

**Internal commands**<br>
All blueshell internal commands starting with a slash (**/**).<br>
<pre>
<b>/help</b>                Show help for blueshell client
<b>/info</b>                Get info about connection state
<b>/put</b> [local-file]    Upload file to server
<b>/get</b> [remote-file]   Download file from server
<b>/quit</b>                Disconnect and quit blueshell
</pre>

**Remote Commands**<br>
All commands without a prefix are executed at the remote machine (server).<br>
You can use any unix/linux command you think of (like ls, mkdir, ...) with some
minor exceptions (ncurses, ...).<br>
**NOTE**: Executing remote commands might be disabled at server side.<br>
<pre>
<b>[blueshell]</b># ls -la
total 40
drwxr-xr-x  5 server server 4096 12 jan 14:59 .
drwxr-xr-x 58 server server 4096 12 jan 10:31 ..
drwxr-xr-x  8 server server 4096 30 jan 07:38 .git
</pre>

**Local Commands**<br>
Commands starting with a **!** will be executed at the local machine.<br>
You can use any unix/linux command you think of (like ls, mkdir, ...) with some
minor exceptions (ncurses, ...).<br>
<pre>
<b>[blueshell]</b># !ls -la
total 40
drwxr-xr-x  5 client client 4096 22 feb 22:52 .
drwxr-xr-x 58 client client 4096 22 feb 16:56 ..
drwxr-xr-x  8 client client 4096 22 feb 22:53 .git
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
