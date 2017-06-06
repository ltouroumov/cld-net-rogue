FROM centos:7

ADD scripts/setup-vm.sh /opt
RUN /opt/setup-vm.sh

CMD /bin/bash
