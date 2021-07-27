FROM python:3.9.6-alpine3.14

RUN apk update

WORKDIR /opt

COPY requirements.txt ./

RUN pip3 install -r requirements.txt

CMD ["uvicorn", "api:app",  "--reload"]

EXPOSE 8000