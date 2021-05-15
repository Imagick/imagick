# Start from the latest NixOS unstable.
FROM nixos/nix
RUN nix-channel --add https://nixos.org/channels/nixpkgs-unstable nixpkgs
RUN nix-channel --update

# Prepare the build environment.
# This is appropriate for production or CI containers, but for
# development environments, it's better to leave the shell.nix
# where it is, so that updates to it don't require a rebuild of the
# container, as that is annoying to do.
# COPY shell.nix /opt
# RUN nix-shell /opt/shell.nix --run 'echo OK'

# Keep a symlink to ImageMagick headers.
RUN nix-build '<nixpkgs>' -A imagemagick7.dev -o /opt/imagemagick

# Prepare the README.
COPY README.md /opt/
RUN sed -i \
  -e "s|@@IM_DEV@@|$(readlink -f /opt/imagemagick-dev)|g" \
  /opt/README.md

# Make the default directory the mounted source code.
WORKDIR /var/app

# Run the process that this container will serve.
CMD tail -f /opt/README.md
