import xmlrpc.client

proxy = xmlrpc.client.ServerProxy('http://localhost:7500/')

r = proxy.factorial_rpc(3)

print("Factorial : ",r)
