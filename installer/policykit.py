import dbus
import os

## highly inspired from this code
## https://fedorahosted.org/python-slip/browser/slip/dbus/polkit.py
#
#stuff = {"0": ("org.freedesktop.PolicyKit", "/", "org.freedesktop.PolicyKit"),
#         "1": ("org.freedesktop.PolicyKit1", "/org/freedesktop/PolicyKit1/Authority", "org.freedesktop.PolicyKit1.Authority")}
#
#authorization = None
#
#bus = dbus.SystemBus()
#for (version, info) in stuff.iteritems():
#    (name, path, interface) = info
#    try:
#        interface = dbus.Interface(bus.get_object(name, path), interface)
#        print "Got interface %s" % interface
#        authobj = bus.get_object("org.freedesktop.PolicyKit.AuthenticationAgent", "/")
#        print "Got authobj %s" % authobj
#        authorization = dbus.Interface(authobj, "org.freedesktop.PolicyKit.AuthenticationAgent")
#        print "Got authorization %s" % authorization
#        # print "Got interface %s from version %s %s" % (interface, version, info)
#    except dbus.DBusException, e:
#        print "Well %s failed" % (name)
#
#if authorization != None:
#    action_id = ':org.freedesktop.hal.storage.crypto-setup-fixed'
#    obtained = authorization.ObtainAuthorization(action_id, dbus.UInt32(0), dbus.UInt32(os.getpid()))
#    if not obtained:
#        raise Exception("failed to get authorization!")
#else:
#    print "Could not get authorization object"

def get():
    import dbus
    bus = dbus.SessionBus()
    x = bus.get_object('org.freedesktop.PolicyKit.AuthenticationAgent', '/', 'org.freedesktop.PolicyKit.AuthenticationAgent')
    return dbus.Interface(x, 'org.freedesktop.PolicyKit.AuthenticationAgent')

import os

# x11 id can be 0 I guess
# x11_window_id = 230686756
x11_window_id = 16777252
# x11_window_id = 0
this_process_id = os.getpid()
action_id = 'org.freedesktop.systemtoolsbackends.set'

print get().ObtainAuthorization(action_id, dbus.UInt32(x11_window_id), dbus.UInt32(this_process_id))
