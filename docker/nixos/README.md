The NixOS container is now available. Get a development shell with:

    docker-compose exec nixos nix-shell /opt/shell.nix

You should configure PHP Imagick with:

    phpize
    ./configure --with-imagick=@@IM_DEV@@
