import xmlrpc.client

proxy = xmlrpc.client.ServerProxy('http://localhost:7500/')

print("factorial of 3 is : %s" % str(proxy.factorial_rpc(3)))