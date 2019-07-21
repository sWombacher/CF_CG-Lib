sudo xhost +local:docker

sudo docker run --rm -it --privileged -v /tmp/.X11-unix/:/tmp/.X11-unix -e DISPLAY=$DISPLAY -v /run/udev:/run/udev -v /run/dbus:/run/dbus cfcg
