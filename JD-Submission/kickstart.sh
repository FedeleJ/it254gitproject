#!/bin/bash
#Created by:  Matthew Macris & Justin Duke
#Created on:  6/03/2020
#Version:     1.0
#Description: Midterm Kickstart Script - Updates and Upgrades a Ubuntu system. Then, installs and uses Docker to deploy the web app WordPress.

#DO NOT MODIFY, set to null as a check case for if docker is fully up and running.
dkrTest=NULL

#Remove the menu.lst config. to fix a conflict between menu.lst updates.
sudo rm /boot/grub/menu.lst
# Generate a new configuration file. 
sudo update-grub-legacy-ec2 -y

#Upgrades the instance.
sudo apt update
sudo apt-get dist-upgrade -qq --force-yes

#Loops until our dkrTest variable reads as "running": keeps retrying until docker is installed and running.
while [ "$dkrTest" != "running" ]; do
    #Installs prerequisite packages, letting apt use packages over HTTPS
    yes | sudo apt install apt-transport-https ca-certificates curl software-properties-common

    #Adds the GPG key for the offical Docker repository
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

    #Adds the Docker repository to the APT sources
    sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu bionic stable"

    #Updates then Upgrades the instance with the added repositories, without needed user input.
    sudo apt update
    sudo yes | apt upgrade 

    #Installs from the Docker repository.
    apt-cache policy docker-ce

    #Installs Docker
    yes | sudo apt install docker-ce

    #gives the system 5 seconds to finish setting everything up. (Probably not needed, but have it just to be safe)
    sleep 5s
    
    #sets the dkrTest variable to the result of our docker status check
    dkrTest=`sudo systemctl status docker | grep 'running' | awk -F" " 'NR==1 { print $3 }' | tr -d "()"`
done

#Install docker-compose
yes | sudo apt install docker-compose
echo "Docker Install succeeded"

#Creates wordpress directory and changes into that directory.
mkdir ~/wordpress && cd ~/wordpress

#Downloads and installs a MariaDB container
sudo docker run -e MYSQL_ROOT_PASSWORD=wordpress -e MYSQL_DATABASE=wordpress --name wordpressdb -v "$PWD/database":/var/lib/mysql -d mariadb:latest

#Uses Docker to pull the latest Wordpress image.
sudo docker pull wordpress

#Downloads and configures a Wordpress container.
sudo docker run -e WORDPRESS_DB_PASSWORD=wordpress --name wordpress --link wordpressdb:mysql -p 80:80 -v "$PWD/html":/var/www/html -d wordpress

#Gets Wordpress server's Public IP, saves it as a variable and echos it out to the User. 
pubIP=`dig +short myip.opendns.com @resolver1.opendns.com`
echo "WordPress install succeeded! go to $pubIP to finish WordPress setup."