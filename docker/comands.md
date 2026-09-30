# 1. Construimos la capa
docker build -t capa2 .

# 2. Creamos la red virtual 
docker network create redes2027

# 3. Receptor
docker run -it --rm --name receptor --network redes2027 \
  --cap-add=NET_RAW capa2 ./receptor

# 4. Emisor
docker run -it --rm --name emisor --network redes2027 \
  --cap-add=NET_RAW capa2 ./emisor