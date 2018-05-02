# The Linux Programming Interface

We are going to hack the hell out of Debian in here! And hopefully, in the
future, we will move to some other cool distros - Alpine and Red Hat may be a
couple fun ones.

## Resources
For right now here are some resources we will be using...

* [How do I get and modify the source code of packages installed through apt-get?](https://askubuntu.com/questions/28372/how-do-i-get-and-modify-the-source-code-of-packages-installed-through-apt-get)
  This comes in handy often so let's just get it ut of the way...

  To get the source for a package simply run
  ```
  apt-get source <pkg>
  ```
  
  If you are told something along the lines of:
  ```
  Reading package lists... Done
  E: You must put some 'source' URIs in your sources.list
  ```
  then go on `/etc/apt/sources.list`. Usually you willbe looking to uncomment
  whatever URI is related to your package and starts with `deb-src`.

  After this, run `sudo apt-get update -y` and try again.


* [How do I install a .deb file via the command line?](https://askubuntu.com/questions/40779/how-do-i-install-a-deb-file-via-the-command-line)
