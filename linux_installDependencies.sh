#!/bin/bash
declare -A osInfo;
osInfo[/etc/redhat-release]=yum
osInfo[/etc/arch-release]=pacman
osInfo[/etc/debian_version]=apt-get

for f in ${!osInfo[@]}
do
    if [[ -f $f ]];then
        echo Package manager: ${osInfo[$f]}

		read -p "Is this your package manager (y/n)? " answer
		case ${answer:0:1} in y|Y )
			case ${osInfo[$f]} in yum)
				sudo yum install glm-devel opencv-devel cmake gcc gcc-c++ freeglut-devel libXi-devel libXmu-devel swig python3-devel
			;;
			pacman)
				sudo pacman -S glm opencv cmake gcc libxmu libxi freeglut swig python make hdf5 vtk gtk3 glew
			;;
			apt-get)
				sudo apt-get install libglm-dev libopencv-dev cmake freeglut3-dev libxmu-dev libxi-dev gcc g++ swig python3-dev
			;;
			*)
				echo Packagemanager not supported
				echo Please install \'glm\' \'opencv\' and \'cmake\' manually
			;;
			esac
	    ;;
	    *)
			echo
	    ;;
		esac
    fi
done

