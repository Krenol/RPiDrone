from jsonmerge import merge
from fastapi import FastAPI
import json
from models.config import Config
import os

app = FastAPI()

def json_file():
    directory_path = os.getcwd()
    folder_name = os.path.basename(directory_path)
    json_file = directory_path.replace(folder_name, 'conf/config.json')
    return json_file

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