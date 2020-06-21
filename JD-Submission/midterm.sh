#!/bin/bash
#Created by:  Matthew Macris & Justin Duke
#Created on:  5/28/2020
#Version:     1.0
#Description: Midterm - spins up a Linux (Ubuntu) system on Amazon AWS. Creates a Security Key, and Security Group Policy for SSH with your current machine.  Pushes a Kickstart script onto the new
#             instance, and executes.  Using Docker then deploys the web app WordPress.

# Sets up variables to be customized as desired.
hostIP=`curl https://checkip.amazonaws.com/`"/32"
filePath=~/environment/midterm
instNm=MidtermInstance-$(( $RANDOM %88 + 11 ))
imageID=ami-085925f297f89fce1
key=midtermKey
region=us-east-1
usrnm=ubuntu
#Name and location of desired kickstart script.
kickstart=kickstart.sh
kickLoc=~/environment/midterm
#DO NOT MODIFY, set to null as a check case for if the new instace is fully up and running.
passChk=NULL

# Creates a Security Group with the name 'midtermGroup'.
aws ec2 create-security-group --group-name midtermGroup --description "Security Group for Justin-Matthew-Midterm"

#finds the Newly created GroupID and saves it as a variable.
secGrp=`aws ec2 describe-security-groups --group-names midtermGroup | grep GroupId | awk '{print $2}' | tr -d '"',','`
echo "Created a Security Group with the name of $secGrp."

#Alters the security group, allowing an SSH connection from the user's current IP Address, and HTTP access.
aws ec2 authorize-security-group-ingress --group-id $secGrp --protocol tcp --port 22 --cidr $hostIP
aws ec2 authorize-security-group-ingress --group-id $secGrp --protocol tcp --port 80 --cidr '0.0.0.0/0'
aws ec2 authorize-security-group-ingress --group-id $secGrp --ip-permissions IpProtocol=tcp,FromPort=80,ToPort=80,Ipv6Ranges='[{CidrIpv6=::/0}]'
echo "Opened port 22 for SSH on Security Group and port 80 for HTTP access for WordPress in $secGrp."

#Creates a new key and saves it in a newly created directory.
mkdir $filePath/KEY
aws ec2 create-key-pair --region $region --key-name $key --query 'KeyMaterial' --output text > $filePath/KEY/$key.ppk
echo "Generated key pair with a name $key."

#Sets permissions for the Private Key file and Private Key.
sudo chmod 600 $filePath/KEY/$key.ppk
sudo chmod 755 $filePath/KEY

#Starts up an instance of the selected imageID.
aws ec2 run-instances --image-id $imageID --tag-specifications "ResourceType=instance,Tags=[{Key=Name,Value=$instNm}]" --key-name $key --instance-type t2.micro --region $region --security-group-ids $secGrp --count 1
echo "Created new instance called $instNm."

#Waiting for initilization.
echo "Please wait. Your instance $instNm is being initialized.  This can take several minutes. Thank you."
#Loops through (with a 5sec delay), checking for a passed status on our instance (verifying it is up and running).
while [ "$passChk" != "passed" ]; do
	sleep 15s
	echo  "Thank you for your patience. Your instance $instNm is almost ready."
	newId=`aws ec2 describe-instances | grep -B 100 "$instNm"\
	 | grep "InstanceId" | awk -F\" '{ print $4 }'`

	passChk=`aws ec2 describe-instance-status --instance-ids "$newId"\
	 --filter Name=instance-status.reachability,Values=passed\
	 | grep passed | awk -F\" 'NR==1 { print $4 }'`
done

#Additional 5sec delay to make sure the system is fully up and running (no trust in AWS reporting systems).
sleep 5s

#Parses through all running instances and looks for one with the name gaven, then saves that instances IPaddress as a new variable.
newIP=`aws ec2 describe-instances\
 --filter "Name=instance-state-name,Values=running"\
 --query "Reservations[*].Instances[*].[PublicIpAddress, Tags[?Key=='Name'].Value|[0]]"\
 | grep -B 1 $instNm | awk -F\" 'NR==1 { print $2 }'`


#Creates a dirctory on the new instance then transfers over the kickstart script to the new directory on the new instance.
sudo ssh -o "StrictHostKeyChecking=no" -i $filePath/KEY/$key.ppk $usrnm@$newIP 'mkdir ~/scripts'
sudo scp -o "StrictHostKeyChecking=no" -i $filePath/KEY/$key.ppk $filePath/$kickstart $usrnm@$newIP:~/scripts/$kickstart

#Sets the kickstart script to executable and then runs it.
sudo ssh -o "StrictHostKeyChecking=no" -i $filePath/KEY/$key.ppk $usrnm@$newIP "sudo chmod +x ~/scripts/$kickstart"
sudo ssh -o "StrictHostKeyChecking=no" -i $filePath/KEY/$key.ppk $usrnm@$newIP "sudo ~/scripts/$kickstart"

#Reports to the User any relevant SSH/Login Information for their new Instance.
echo "A text file has been created in your current directory with the information you need to SSH into this newly created instance"
printf "*NOTE* To SSH into your new instance: \nUsername: $usrnm \nKey File: $filePath/Key/$key.ppk \nNew IPAddress: $newIP" >> ./$instNm-SSH.txt 