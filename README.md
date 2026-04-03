gcc -Wall -Wextra -Werror -o ft_ping ft_ping.c
alias bash_sudo='docker run -it --cap-add=NET_RAW -v $(pwd):/ft_ping ubuntu bash'
