# 1. Construimos la imagen del docker
docker build -t capa2 .

# 2. Creamos la red virtual 
docker network create practica2redes

# 3. Receptor
docker run -it --rm --name receptor --network practica2redes \
  --cap-add=NET_RAW capa2 ./receptor

# 4. Emisor
docker run -it --rm --name emisor --network practica2redes \
  --cap-add=NET_RAW capa2 ./emisor