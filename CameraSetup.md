Camera Setup
=======================
Since we are using a USB 2.0 CMOS camera, some libraries will need to be installed for the camera to configure properly.
See below for the official documentation on the setup:
https://github.com/TheImagingSource/tiscamera/wiki/Getting-Started-with-USB-2.0-cameras-on-a-Raspberry-PI-or-on-other-Linux-Systems/1637854cb8564add53b7914064033233736c2b54 
Note that the above link leads to an older document that is now deprecated because USB 3.0 CMOS camera does not require additional configurations.
=======================
### Table of Contents

* [Setup](#setup)
    - [General](#general)
    - [Aravis](#aravis)
* [Issues](#issue)

### Setup

#### General
```
git clone https://github.com/theimagingsource/tiscamera -b gstreamer-0.10 tiscamera-0.10 //downloads GStreamer from official repo
sudo apt-get install autoconf libglib2.0 libudev-dev libgstreamer0.10 gstreamer0.10-plugins-base gstreamer0.10-plugins-good gstreamer0.10-plugins-bad gstreamer0.10-tools gstreamer-plugins-base0.10-dev
cd path/to/tiscamera/src
./bootstrap.sh
./configure --enable-aravis //aravis installation shown below
make
make install
```

#### Aravis
You will need to download a stable release from the official project repo. 
(At the time of writing this document, stable release is version 0.4 and can be found at http://ftp.gnome.org/pub/GNOME/sources/aravis/0.4/)
Official documentation regarding how Aravis is used by the camera is found at https://github.com/TheImagingSource/tiscamera/wiki/Aravis.
Official documentation regarding Aravis library is found at https://github.com/AravisProject/aravis.

```
sudo apt-get install autoconf intltool python-gobject-dev gobject-introspection gtk-doc-tools libgstreamer0.10-dev python-gst0.10-dev libxml2-dev //installing dependencies
cd path/to/downloaded/aravis/directory
autogen.sh
make
sudo make install
```

### Issues
Placeholder for future installation issues and solutions.