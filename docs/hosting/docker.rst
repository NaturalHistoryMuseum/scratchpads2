Hosting your own Scratchpad
===========================

*You can very easily host a Scratchpad on your own server using our in-built Docker configuration.*

Finding a server
~~~~~~~~~~~~~~~~

To host your own Scratchpad you need access to a Linux Server (we prefer the Ubuntu distro) with admin permission so you can install packages. 

There are plenty of companies providing this service - just search for Ubuntu Virtual Private Server.  Or your institution might be able to provide a VPS for you.    

Once you have a server, SSH in and we'll start installing the packages you need to run your Scratchpad. 


Installing Docker
~~~~~~~~~~~~~~~~~

These instructions are based on https://docs.docker.com/engine/install/ubuntu/


1. Update the apt package index and install packages to allow apt to use a repository over HTTPS:

    .. code-block:: console

        sudo apt-get update
        sudo apt-get install ca-certificates curl gnupg

2. Add Docker’s official GPG key:

    .. code-block:: console

        sudo install -m 0755 -d /etc/apt/keyrings
        curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
        sudo chmod a+r /etc/apt/keyrings/docker.gpg

3. Set up the repository

    .. code-block:: console
        
        echo \
        "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
        "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
        sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

        

