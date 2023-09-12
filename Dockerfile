FROM python:latest
RUN apt-get update && apt-get install build-essential && pip install locust
RUN mkdir /code 
WORKDIR /code
COPY . /code/
RUN chmod +x /code/entrypoint.sh
