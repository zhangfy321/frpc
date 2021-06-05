## consul
- 开启一个集群，要求有3个server，同时开启管理ui

`docker run -d --name=consul1 -p 8900:8500 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=true
--bootstrap-expect=3 --client=0.0.0.0 -ui`

- 开启其余两个server

`docker run -d --name=consul2 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=true --client=0.0.0.0
--join 172.17.0.2 `

`docker run -d --name=consul3 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=true
--client=0.0.0.0 --join 172.17.0.2`

- 开启一个client

`docker run -d --name=consul4 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=false --client=0.0.0.0
--join 172.17.0.2 `

## ELK
## k8s
## grafana
