FROM python:latest
RUN apt-get update && apt-get install -y build-essential
RUN mkdir /code 
WORKDIR /code
COPY . /code/
RUN chmod +x entrypoint.sh
