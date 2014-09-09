# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"
Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = "scratchpads/debian7"
  config.vm.network "public_network"
  config.vm.provision :shell, path: "bootstrap.sh"
  config.vm.network "forwarded_port", guest: 80, host: 8888

  # Virtualbox specific configuration
  config.vm.provider "virtualbox" do |vb|
    vb.customize ["modifyvm", :id, "--memory", "1024"]
  end
end
