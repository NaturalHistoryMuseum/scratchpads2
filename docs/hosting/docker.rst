Hosting your own Scratchpad
===========================

*You can very easily host a Scratchpad on your own server using our in-built Docker configuration.*

Finding a server
~~~~~~~~~~~~~~~~

To host your own Scratchpad, you need access to a Linux Server (we prefer the Ubuntu distro) with admin permission so you can install packages. 

There are plenty of companies providing this service - just search for Ubuntu Virtual Private Server.  Or your institution might be able to provide a VPS for you.    

Once you have a server, SSH in, and we'll start installing the packages you need to run your Scratchpad. 


Installing Docker
~~~~~~~~~~~~~~~~~

These instructions are based on https://docs.docker.com/engine/install/ubuntu/


1. Set up the repository
########################

1.1 Update the apt package index and install packages to allow apt to use a repository over HTTPS:

    .. code-block:: console

        sudo apt-get update
        sudo apt-get install ca-certificates curl gnupg

1.2 Add Docker’s official GPG key:

    .. code-block:: console

        sudo install -m 0755 -d /etc/apt/keyrings
        curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
        sudo chmod a+r /etc/apt/keyrings/docker.gpg

1.3 Set up the repository

    .. code-block:: console

        echo \
        "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
        "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
        sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

        
2. Install Docker
#################

2.1 Update the apt package index:

    .. code-block:: console

        sudo apt-get update

2.2 Install Docker Engine, containerd, and Docker Compose:

    .. code-block:: console

        sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin


3. Install Scratchpads
######################        

3.1 Clone Scratchpads

     .. code-block:: console

        git clone https://github.com/NaturalHistoryMuseum/scratchpads2.git /var/lib/scratchpads2

3.2 Update environment variables

    .. code-block:: console

        cp /var/lib/scratchpads2/.env.template /var/lib/scratchpads2/.env

You must update the SQL default settings in the .env file (if you are recreating your site from an existing Scratchpads site, the settings.php will have sensible values to use).

    .. code-block:: console    

        MYSQL_ROOT_PASSWORD=root
        MYSQL_DATABASE=drupal
        MYSQL_USER=drupal
        MYSQL_PASSWORD=drupal

3.3 Run docker images

    .. code-block:: console    

        cd /var/lib/scratchpads2
        make up


4. Recreate from backup
#######################

If you want to recreate your hosted Scratchpad, first download the backup file following these instructions :doc:`/export/create-backup`.

    .. code-block:: console    

        make site-from-archive archive=/path/to/backup/backup.myspecies.info-20230404.194808.tar.gz

5. Daemonize
############

To keep serving the site once you log-off from the server, you must daemonize the process.

    .. code-block:: console    

        make down
        docker compose -f docker-compose.yml -f docker-compose.production.yml up -d

6. Set up cron
##############

Scratchpads has tasks that run at intervals in the background (indexing for the site search, for example). To enable this, you must add a cron task.

    .. code-block:: console  

        crontab -e

And enter the text before saving the file. 

    .. code-block:: console  

        0 3 * * * docker exec scratchpads.apache drush core-cron > /var/log/docker.cron.log 2>&1