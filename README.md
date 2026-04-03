To run root user inside a docker environment for testing:
alias bash_sudo='docker run -it --cap-add=NET_RAW -v $(pwd):/ft_ping ubuntu bash'
