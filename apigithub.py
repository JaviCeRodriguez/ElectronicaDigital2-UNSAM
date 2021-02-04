import requests

resp = requests.get('https://api.github.com/users/JaviCeRodriguez')
if resp.status_code != 200:
    # This means something went wrong.
    print('GET {}'.format(resp.status_code))
else:
    print(dict(resp.json()).keys())
    print(dict(resp.json()).values())
    # for item, value in resp.json():
    #     print(item.value)