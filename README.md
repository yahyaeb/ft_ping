To run root user inside a docker environment for testing:

alias bash_sudo='docker run -it --cap-add=NET_RAW -v $(pwd):/ft_ping ubuntu bash -c "apt-get update -y && apt-get install -y gcc make && exec bash"'

ping -v -t 1 google.com