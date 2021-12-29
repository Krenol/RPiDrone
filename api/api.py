from jsonmerge import merge
from fastapi import FastAPI
import json
from models.config import Config
import os

app = FastAPI()

def json_file():
    directory_path = os.getcwd()
    json_file = directory_path.replace('/api', '')
    json_file = json_file.replace('/build', '')
    return json_file + '/conf/config.json'

JSON_FILE = json_file() 

@app.get("/config")
async def patch_conf():
    with open(JSON_FILE, 'r') as f:
        data = f.read()
    conf_j = json.loads(data)
    return conf_j

@app.patch("/config")
async def patch_conf(conf: Config):
    with open(JSON_FILE, 'r') as f:
        data = f.read()
    conf_j = json.loads(data)
    result = merge(conf_j, json.loads(conf.json(exclude_unset=True)))
    with open(JSON_FILE, 'w') as f:
        f.write(json.dumps(result, indent=4))
    return result