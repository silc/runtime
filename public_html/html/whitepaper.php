&nbsp;<br />
<b><big>SILC Protocol White Paper</big>
<br />
Version 1.0 / 03 Aug 2001

<br />&nbsp;<br />
Introduction</b><br />&nbsp;<br />

Chat protocols are very popular on the Internet.  They have actually
been very popular since the very first chat protocols appeared on the net.
The Internet Relay Chat (IRC) was one of the first chat protocols, and quickly
gained the status of being the most popular chat on the net.  Today, IRC
has several competitors from various other so called Instant Messaging (IM)
protocols, such as ICQ.  However, all of these different chat protocols
have something in common; they are all insecure.
<br />&nbsp;<br />

The security is important feature in applications and protocols in 
contemporary network environment.  The older chat protocols, however have
failed to meet the growing security requirements on the Internet.
It is not anymore enough to just provide services, like for example
chat services. Now, they need to be secure services.
<br />&nbsp;<br />

The Secure Internet Live Conferencing (SILC) protocol is a new generation
chat protocol which provides full featured conferencing services, just
like any other contemporary chat protocol provides.  In addition, it
provides security by encrypting and authenticating the messages in
the network.  The security has been the primary goal of the SILC protocol
and the protocol has been designed from the day one security in mind.
All packets and messages travelling in the SILC Network are always
encrypted and authenticated.  The network topology is also different
from for example IRC network.  The SILC network topology attempts to be
more powerful and scalable than the IRC network.  The basic purpose
of the SILC protocol is to provide secure conferencing services.
<br />&nbsp;<br />

The SILC Protocol have been developed as Open Source project.  The
protocol specifications are freely available and they have been submitted to
the IETF.  The very first implementations of the protocol are also already
available.

<br />&nbsp;<br />
<b>About This White Paper</b><br />&nbsp;<br />

The purpose of this white paper is to give short but deep enough introduction
to the SILC Protocol.  The document describes the purpose of the protocol
and how the protocol works in practice.  This document is intended for all
audience.  This document should be easy to understand for non-technical
person and still be detailed enough for technically oriented person.  See
the section <a href="#terms" class="normal">Terms and Abbreviations</a> for terms used
in this document.

<br />&nbsp;<br />
(c) Copyright 2001 Pekka Riikonen 
(<a href="mailto:priikone at silcnet.org" class="normal">priikone at silcnet.org</a>)
<br />&nbsp;<br />
This document is free document; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.  This document is distributed in
the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.


<br />&nbsp;<br />
<b>SILC Protocol</b><br />&nbsp;<br />

The Secure Internet Live Conferencing (SILC) protocol provides secure
conferencing services over insecure network channel.  The SILC is IRC
like protocol, however it does not support IRC.  Strong cryptographic
methods are used to protect SILC packets inside the SILC network.  SILC
provides all the common conferencing services like channels, channel
messages, private messages, nicknames and various commands.  Difference
to other chat protocol is in the design of the protocol.  The SILC 
protocol has been designed from the day one security in mind and it
shows in the protocol design.
<br />&nbsp;<br />

Generally it is assumed that the SILC Network is trusted.  This means
that clients can fully trust the servers and routers in the SILC Network.
In real life this is not always possible.  In the Internet it is possible
that some server or router would get compromised by a malicious
cracker.  However, if the SILC Network is closed network, for example
inside a orgranization the assumption generally is true.  The SILC
protocol is secure even if the end users consider the network
untrusted, and provides several ways to still have secure conversation
on the SILC Network.
<br />&nbsp;<br />

The packets in the SILC network are always encrypted.  It is not possible
to send unencrypted messages in SILC.  This assures that end user cannot
even accidently send unencrypted messages while thinking that it is
encrypted.  This is the problem of most other chat protocols that provide
so called plugin encryption.  They are not secure by default but try
to provide security by applying external security protocol such as PGP
or SSL.  In these cases the security is achieved usually by encrypting the
data while key management and other security issues may be left out, leaving
the implementation vulnerable to various security problems.  The other
problem is also that the external protocols tend to leave the network
only partly secured; usually only two points in the network are secured
with for example SSL.  While SSL does provide provable security it is not
enough to provide security for a chat network as a whole.
<br />&nbsp;<br />

The network topology is also different to various other chat protocol,
like for example IRC.  IRC has tree style network where SILC has so
called cellular network.  A cell consists of a router, servers and clients.
The cell can also have backup routers in case the private router becomes
unresponsive.

<br />&nbsp;<br />
<img src="img/silc_network.png" alt="( SILC Network - IMAGE )" />
<br />&nbsp;<br />

The diagram above illustrates a portion of the SILC network.  It shows
two cells that both has several servers, and backup routers and several
clients.  Clients can connect to server and routers if they want to.
The following sections will describe the entities of the SILC Network
in greater detail.

<br />&nbsp;<br />
<b>Clients</b><br />&nbsp;<br />

A client is a piece of software connecting to SILC server.  The software
is usually run by the end user, a real person that is.  The purpose of the
clients is to provide the end user an interface to the SILC services.
They are used to actually engage the conversations on the SILC Network,
and they can be used to execute various SILC commands.
<br />&nbsp;<br />

The clients are distinquished from other clients by unique Client ID.
There cannot be multiple same Client IDs in the SILC Network at the same time.
The end user, however does not use Client IDs.  The end users usually selects
a perferred nickname they want to use, and identifies themself with that
nickname to other users on the network.  The nicknames are not unique in
the SILC Network.  There can be multiple same nicknames at the same time
on the network.  The maximum length for the nickname is 128 characters.
<br />&nbsp;<br />

Most of the other chat protocols have unique nicknames.  This is where SILC
differs from most of the other chat protocols.  The purpose of this
feature is to make IRC style nickname wars obsolete, as no one owns their
nickname; there can always be somene else with the same nickname.
<br />&nbsp;<br />

When client connects to the server the SILC Key Exchange (SKE) protocol and
SILC Connection Authentication protocol are executed.  The result of the
SKE protocol is the session key that the client and server use to secure
their communication.  All commands, for example, that the client sends
to the server are secured with the session key.  The session key expires
periodically and the rekey process can be executed with or without the
Perfect Forward Secrecy (PFS).  The connection authentication protocol is
used to authenticate the client to the server.  The server may allow the
client to connect without authentication, or it may require a passphrase or
public key based authentication.


<br />&nbsp;<br />
<b>Servers</b><br />&nbsp;<br />

Servers forms the basis for the SILC Network, by providing a point to which
clients may connect.  There are two kinds of servers in SILC; normal servers
and router servers.  The next section describes the function of router
server.
<br />&nbsp;<br />

Normal servers connect to router server.  Normal servers cannot directly
connect to other normal servers.  Messages that are destined outside the
local server are always sent to the router for further routing.
The clients usually connect to the normal server, however, clients may
connect to router servers as well.  The SILC Network diagram above
illustrates how normal servers connects to the router server.
<br />&nbsp;<br />

The servers are distinquished by other servers in the network by unique
Server ID.  There cannot be multiple same Server IDs in the SILC Network
at the same time.  The servers keep track of local information.  It knows
all locally connected clients and it knows all channels that its clients
have joined.  However, it does not know any global information.  It
usually does not keep track of global clients, however, it may cache
that information if it was queried.  The reason for this is that the
server does not need to keep global information up to date and thus
makes the server faster (and in the end the entire network faster).
They can always query the information from the router.
<br />&nbsp;<br />

When server connects to its router the SILC Key Exchange (SKE) protocol
and the SILC Connection Authentication protocol are executed, just like
when client connects to server.  The SKE results in to the session key
that is used to secure the communication between the server and the
router.  The connection authentication protocol is used to authenticate
the server to the router.  The authentication is always based in either 
passphrase or public key.


<br />&nbsp;<br />
<b>Routers</b><br />&nbsp;<br />

The router servers are servers that actually handles the message routing
in the network.  They are, however also normal servers and they do accept
client connections.  Each of the router in the network is called a cell.
A cell can have only one active router and it may have several servers
and several clients.  The cell, however may have backup routers that can
take over the tasks of the primary router if it becomes unresponsive.
The switch to the backup router should be transparent and only local
connections to the primary router are lost.  Other connections in the
cell are intact, and clients and servers merely experience some lag in
the network connection during the switch to the backup router.
<br />&nbsp;<br />

The normal server knows only local information.  Router server on the
other hand knows local information and global information.  It considers
the cell as local and outside cells as global.  It knows all the clients
connected to the network, all created channels, and all routers and servers
in the network.  The server may query the global information if it is needed.
For example, when client sends WHOIS command, the server may query the
information from the router.  If the router does not know all the details
that the WHOIS command requires it can query the information from a router
or a server which knows all the details.  It may then cache that information.
<br />&nbsp;<br />

The primary purpose of the router server is to route the messages to
local servers and local clients, and messages that are destined to outside
the cell are routed to the primary route or some other secondary
route if it is a faster route.  The routers in the network forms a ring.
Each router has a primary route to other router in the network.  Finally
the ring is closed by the last router using the first router in the
network as its primary route.

<br />&nbsp;<br />
<img src="img/silc_routers.png" alt="( SILC Routers - IMAGE )" />
<br />&nbsp;<br />

The diagram above illustrates how the routers forms a ring in the network.
A router may have several secondary routes which it may use when it
routes the packets.
<br />&nbsp;<br />

When routers connect to its primary router the SKE and the SILC Connection
Authentication protocols are executed just like when normal server connects
to its router.  The session key is used to secure the communication between
the routers.  All the secondary routes also have their own session keys.


<br />&nbsp;<br />
<b>SILC Packet Protocol</b><br />&nbsp;<br />

The basis of SILC protocol relies in the SILC packets and they are with
out a doubt the most important part of the protocol.  The SILC Packet 
protocol is a binary packet protocol.  The protocol provides secure
binary packets and assures that the contents of the packets are secured
and authenticated.
<br />&nbsp;<br />

Packets are used in the SILC protocol all the time to send for example
channel messages, private messages, commands and other information.  All
packets in SILC network are always encrypted and their integrity is
assured by computed Message Authentication Codes (MAC).  The protocol
defines several packet types and packet payloads.  Each packet type
usually has a specific packet payload that actually defines the contents
of the packet.  Hence, the actual data in the packet is the packet payload 
defined in the protocol.

<br />&nbsp;<br />
<img src="img/silc_packet.png" alt="( Typical SILC Packet - IMAGE )" />
<br />&nbsp;<br />

As the diagram above illustrates the SILC packet is constructed from the
SILC Packet Header that is included in all SILC packets, data area that
includes the packet payloads, and MAC area which assures the integrity of the
packet.  Entire SILC packet is always encrypted, except for the MAC area
which is never encrypted.  The encryption process and the key used,
however depends on the packet payload.  Some of the payloads are encrypted
with the session key and some are encrypted with other keys, for example
with channel message keys.  The SILC Packet Header is always encrypted with
the session key.  The MAC is computed from the SILC Packet Header and the
data area before encrypting the packet.


<br />&nbsp;<br />
<b>SILC Key Exchange Protocol</b><br />&nbsp;<br />

SILC Key Exchange Protocol (SKE) is used to exchange shared secret
between connecting entities.  The result of this protocol is a key material
used to secure the communication channel.  This protocol is executed when,
for example client connects to server.  It is also executed when server
connects to router.  And, there is no reason why it could not be executed
between two clients too, if two clients would need to create secret key.
The purpose of the SKE protocol is to create session keys to be used
in current SILC session.  The SKE is based on the Diffie-Hellman key
exchange algorithm, and is immune to man-in-the-middle attack.
<br />&nbsp;<br />

This is the first protocol that is executed when creating connection to,
for example SILC server.  All the other protocols are always executed
after this protocol.  This way all the other protocols are secured since
the SKE creates the session key that is used to secure all subsequent
packets.  The session keys created in the SKE are valid only for some
period of time (usually an hour) or at most until the session ends.
The rekey process can be executed with or without the Perfect Forward
Secrecy (PFS).
<br />&nbsp;<br />

The security properties that are used in the SILC session are also
negotiated during the SKE.  The protocol has initiator and responder.
The initator is the one who starts the SKE negotiation and responder is
the one who receives the SKE negotiation.  When the protocol is started
initiator sends a list of security properties that it supports.  The
responder then selects the security properties it supports and sends
its reply to the initiator.  The security properties includes ciphers,
hash functions, public key algorithms, HMAC functions and other
security properties.  The responder can always choose the properties
it supports.
<br />&nbsp;<br />

After the security properties are selected the protocol continues
by performing the Diffie-Hellman key exchange algorithm.  At the same
time the intiator and responder also sends their public keys or
certificates to each other.  The responder also computes a signature
that the initiator will verify.  It is also possible to perform a
mutual authentication where both of the parties computes a signature
which are verified by each other independently.  If any of the phases
of the protocol are to fail the connection is closed immeadiately.
<br />&nbsp;<br />

The public key or certificate that is received during the SKE protocol
must be verified.  If it is not verified it would be possible to 
execute a man-in-the-middle attack against the SKE protocol.  If
certificates are used they can be verified by a third party Certification
Authority (CA).  Verifying a public key requires either confirming
a fingerprint of the public key over phone or email, or the server
can for example publish the fingerprint (and the public key) on some 
website.  In real life systems accepting the public key without
verification, however is often desired.  In many security protocols,
such as in SSH2, the public key is accepted without verification
in the first time when the connection is created.  The public key is
then cached on local hard disk.  When connecting next time to the
server the public key on local cache is verified against the public
key server sent.  In real life this works most of the time.  However,
if client (or server) cannot trust this, it must find some other way
to verify the received public key or certificate.


<br />&nbsp;<br />
<b>SILC Connection Authentication Protocol</b><br />&nbsp;<br />

Purpose of SILC Connection Authentication protocol is to authenticate the
connecting party with server or router.  This protocol is executed when
for example client connects to server.  It is also executed when server
connects to router.  Its other purpose is to provide information for the
server about which type of connection it is.  The type of the connection
defines whether it is client, server or router.  If it is client then
the server will create a new Client ID for the client.  If it is server
then it will except the server to send its Server ID.  Server IDs are
created by the servers and routers itself.
<br />&nbsp;<br />

Since the SILC Connection Authentication protocol is always executed after
the SKE protocol, session keys has been established already.  This means
that all packets sent in the connection authentication protocol are encrypted 
and authenticated.
<br />&nbsp;<br />

The authentication may be based either in passphrase or public key
encryption.  It is also possible to not require authentication at all.
If the authentication is based to passphrase the passphrase is sent
to the server.  As the packet sent by, for example client, is entirely
encrypted it is safe to send the passphrase inside the packet.
<br />&nbsp;<br />

If the authentication is based to public key then, for example the client, 
signs data with its private key and sends it to the server.  The server
then verifies this signature by using the client's public key.  The
packet is also encrypted in the case of public key authentication.
<br />&nbsp;<br />

If the authentication is to fail the connection to the server or router
will be refused.  If it is successful the connection is granted.  After
this the client is ready to communicate in the SILC Network.


<br />&nbsp;<br />
<b>Channels</b><br />&nbsp;<br />

A channel is a named group of one or more clients which will all receive
messages addressed to that channel.  The channel is created when first
client joins to it, and the channel ceases to exist when the last client
leaves it.  When channel exists, any client can reference it using the 
name of the channel.  Channel is a place where group of people can engage
conversation.
<br />&nbsp;<br />

Channel names are unique in the SILC Network.  There cannot be multiple
same channels in the network at the same time.  However, channel has also
a Channel ID which is actually used to reference the channel in the
SILC Network.  The maximum length for the channel name is 256 characters.
<br />&nbsp;<br />

Channels can have operators that can administrate the channel and operate
all of its modes.  There are two types of operators on the channel:
channel founder and channel operator.
<br />&nbsp;<br />

The channel founder is the client which created the channel.  Channel
founder is channel operator with some more privileges.  Channel founder
can operate all of the channel's modes.  Furthermore, channel founder
privileges cannot be removed by any other operator on channel and channel
founder cannot be removed from the channel by force.  It is also possible
for the channel founder to regain its privileges at later time, even if
they have left the channel.
<br />&nbsp;<br />

Channel operator is operator that can operate most of the channel's
modes and administrate the channel.  However, it cannot operate all
modes which are strictly reserved for channel founder.  Channel operator
is, however able to adminstrate the channel, set some modes on the
channel, remove a badly behaving client from the channel, and promote
other clients to become channel operator.


<br />&nbsp;<br />
<b>Channel Message Delivery</b><br />&nbsp;<br />

All clients that have joined the channel can send messages to the channel.
All channel messages are secured and authenticated by channel key.  The
channel key is generated by the server when the channel is created,
a client joins the channel, or a client leaves the channel.  The channel
key is also regenerated periodically.  The reason for the regeneration
of channel key everytime someone joins or leaves the channel is that
it prevents new clients joining the channel, and old clients leaving the
channel, to encrypt or decrypt old or new messages.  They can encrypt
and decrypt channel messages only when they have joined on the channel.
<br />&nbsp;<br />

Channel keys are cell specific in the SILC Network.  Each cell that
have clients joined on a particular channel have also own key for the
channel.  That key is not shared by other cells in the network.  Inside
the cell the channel key is known by the router and all servers that
have clients on the channel and all clients that have joined the channel.

<br />&nbsp;<br />
<img src="img/silc_channel.png" alt="( Channel Message Delivery - IMAGE )" />
<br />&nbsp;<br />

The diagram above illustrates typical delivery of channel messages inside
a cell and between two cells.  Both of the cells have their own channel
key.  Both cells knows all clients joined on the channel.  When message
is sent to the channel by an client, it is encrypted with the current
channel key in that cell.  The servers and the router in the local cell
then routes the message to all local clients who have joined the channel.
If the channel has clients that belong to other cell in the network the
router will route the channel message to that cell.  When channel
messages are sent between routers they are first decrypted with the
current channel key, and then re-encrypted with the session key shared
between the two routers.  The router who receives the channel message
then decrypts it with the session and re-encrypts it with the
current channel key in that cell.  It then distributes the channel message
to all clients on the channel.  The clients who have joined the channel
always knows the current channel key and can decrypt all channel messages
they receive.  Note that normal servers in the SILC network never decrypt
the channel messages even though the have the key.  There is no reason
for servers to decrypt the message.  The router decrypts the message
only when sending it between two routers.
<br />&nbsp;<br />

This method of channel message delivery is the default way to send
channel messages in the SILC Network.  However, this is not perfect
solution on all circumstances.  If the clients joined on a particular
channel cannot trust, or do not want to trust the servers and routers
in the SILC Network they can consider the fact, that servers and routers
knows the channel key is actually a breach of security.
<br />&nbsp;<br />

If the clients on the other hand can trust their servers and routers
in the SILC Network this is the recommended way of sending channel
messages.  This method is the simplest method for end user since it
does not require any special settings before engaging the conversation
on the channel.  The client merely joins the channel, receives the
channel key from the server and can start the conversation on the
channel.
<br />&nbsp;<br />

In addition of encrypting channel messages it also possible to digitally
sign all sent channel messages.  The receiver could then verify the
signature of each of the message using the sender's public key.


<br />&nbsp;<br />
<b>Channel Message Delivery With Channel Private Key</b><br />&nbsp;<br />

If the clients cannot trust the servers and routers in the SILC Network
they should not use the default way of sending the channel messages.
Instead, they should use channel private keys to encrypt and decrypt
the channel messages.  Channel private keys are keys that are known
only by the clients who have joined the channel.  Sservers and
routers do not know the key and cannot decrypt the messages.  When
message is sent between two routers they are merely re-encrypted with
the session key but not decrypted since the router do not have the
key to do that.
<br />&nbsp;<br />

The clients who have joined the channel must first agree on the channel
private key they are going to use.  The key may generally be anything.
It may be a passphrase or a random string, or the key may negotiated
using some key exchange protocol which provides negotiating the
key for multiple clients at the same time.
<br />&nbsp;<br />

As the channel private key is actually entirely local setting in the
client, it is possible to set several channel private keys for one
channel.  It is possible to have multiple channel private keys that
are not known by all channel members.  When encrypting messages with
one channel private key only the clients who have that key can decrypt
the message.  The other key could be shared for example by all clients
on the channel and thus all clients can decrypt messages encrypted with
that key.  In this way it is actually possible to have a private group
conversation inside the channel while having global conversation at the
same time.


<br />&nbsp;<br />
<b>Private Messages</b><br />&nbsp;<br />

Private messages are messages that are sent from one client to another 
through the SILC Network.  They are private because they are not sent to
anyone else except to the true receiver of the message.  Private messages
can be used to engage private conversation with another client if channels
are not desired.
<br />&nbsp;<br />

As all messages in SILC the private message are also encrypted and
authenticated.  There are several ways to secure private messages.  By
default private messages are encrypted using the session keys established
in the SKE protocol.  It is also possible to negotiate a private message
key between the two clients and encrypt the messages with that key.  It
is even possible to encrypt the messages with public key cryptosystem,
if desired.  The next sections will describe all these private message
delivery methods.

<br />&nbsp;<br />
The SILC protocol provides these three methods of delivering private messages
because none of the methods alone can satisfy the security requirements
of all people.  The end user should decide the acceptable level of risk,
the required level of security and other security and usability aspects when
deciding what way of sending private message suites for them.
<br />&nbsp;<br />

In addition of encrypting private messages it also possible to digitally
sign all sent private messages.  The receiver could then verify the
signature of each of the message using the sender's public key.


<br />&nbsp;<br />
<b>Private Message Delivery With Session Keys</b><br />&nbsp;<br />

Sending private messages are by default secured with session keys established
in the SKE protocol.  This means that the private message is always encrypted
with the session key of the next receiver of the message enroute to the 
receiving client.  This also means that the message is decrypted and
re-encrypted everytime it is sent further to the receiving client.

<br />&nbsp;<br />
<img src="img/silc_priv1.png" alt="( Basic Private Message Delivery - IMAGE )" />
<br />&nbsp;<br />

As the diagram above shows the private messages sent by Client A to the
Client B travels through the SILC Network and is always decrypted and
re-encrypted with the session key of the next receiver.  The Client B then
finally decrypts the private messages that is encrypted with the session
key shared between the Client B and the Server Y.
<br />&nbsp;<br />

This way of securing private messages is not perfect and cannot be used
in all circumstances.  If the clients having the conversation cannot trust
the servers and routers in the SILC Network they should not send private
messages that are secured in this manner.  Messages secured in this manner
can be decrypted by the servers and routers that the clients may consider
to be untrusted.
<br />&nbsp;<br />

If the clients on the other hand trust the servers and routers in their 
SILC Network, or they do not care that servers can decrypt their messages,
sending private messages in this way is very simple from client's point
of view.  For servers and routers this of course means that they need
to decrypt and re-encrypt each private message.  Since this way of securing
private message cannot be used at all times the SILC protocol provides
other ways of securing private messages.


<br />&nbsp;<br />
<b>Private Message Delivery With Private Message Key</b><br />&nbsp;<br />

Private messages can be secured with private message key as well.  This
key is known only by the sender of the message and the receiver of the
message.  This way no one else except the sender and the receiver can encrypt
and decrypt the private messages.  The message is encrypted by the sender
with the private message key and all the servers and routers pass the message
through enroute to the receiver.  They cannot decrypt the message since
they do not have the key.  When sending private messages in this way it
does not matter whether the clients trust or do not trust the servers and
routers in the SILC network.

<br />&nbsp;<br />
<img src="img/silc_priv2.png" alt="( Private Messages with Private Message Key - IMAGE )" />
<br />&nbsp;<br />

As the diagram above shows the Client A encrypts the message with private
message key and sends the message to the SILC Network.  All servers and
routers merely pass the message through since they cannot decrypt it.
The Client B then receives the message and decrypts it with the private
message key.
<br />&nbsp;<br />

Sending private messages in this manner is always secure since the key is
shared only by the sender and the receiver.  The problem of this method
is that the sender and the receiver must somehow agree about the key
they are going to use.  The private message key can generally be anything.
It can be a passphrase that only the sender and the receiver knows.  They
could have been agreed to use some word or phrase as the key sometime
earlier before they started the conversation.  Or the key maybe from some
random string from a code book that only the sender and the receiver poses.
Or it can be a key that is negotiated using some key exchange protocol.
<br />&nbsp;<br />

The problem however is fundamental.  How to agree to use some key when
you cannot reach the other person over secure channel?  The SILC protocol
solves this problem by providing a possiblity to negotiate the key
between two clients using the SKE protocol.  One or both of the clients
can set up the SKE server running in their host and ask the other client
to connect to it.  In this case the SKE is executed outside the SILC
Network.  As a result of the SKE protocol the clients have now shared
secret that they can use as private message key.  The key is known only
by the two clients that executed the SKE protocol.  They can then use
that key to secure all subsequent private messages.
<br />&nbsp;<br />

Using this method of private messages delivery is recommended if the
clients cannot trust the servers and routers in the SILC Network.  The 
drawback is the extra phase of setting the private message key before
starting the conversation.  However, using the SKE protocol is the
recommended way to negotiate the private message key since it can be
automatized and does not cause any extra tasks for end user.


<br />&nbsp;<br />
<b>Private Message Delivery With Public Key Encryption</b><br />&nbsp;<br />

If the clients cannot trust the servers and routers in the SILC Network
they can use the private message key.  As described in the previous section
it is easy to set up with the SKE protocol.  However, sometimes the two
clients do not want to use any passphrases as private message key or
negotiate the key with SKE, or perhaps they are unable to negotiate the
key because of some other external problem.  The SILC protocol provides
yet another way of securing the private messages.  This way does not
require setting or negotiating private message key.  And, in this method
also it does not matter whether the clients trust or do not trust the
servers and routers in the SILC Network.  The method is public key
encryption.  The clients can encrypt the private messages with the
receiver's public key and send the message to the network.  The servers
and routers cannot decrypt the messages since they do not have the
receiver's private key.  The receiver on the other hand has the private
key which it uses to decrypt the message.

<br />&nbsp;<br />
<img src="img/silc_priv3.png" alt="( Private Messges with Public Key Cryptosystem - IMAGE )" />
<br />&nbsp;<br />

As the diagram above shows the Client A has the Client B's public key.
It will encrypt the message with that key and sends the message to the
SILC Network.  All servers and routers pass the message through since
they cannot decrypt it.  The Client B then uses its private key to
decrypt the message.  The Client B has also the Client A's public key 
that it can use to encrypt messages that it will send to Client A.
<br />&nbsp;<br />

Even this method of private message delivery is not perfect.  The drawbacks
of this method is that the public key encryption process, as being
asymmetric cryptosystem, is much slower than encryption process with
symmetric cryptosystems.  This is not probably problem with short messages
but may be inconvenient with long messages.  The other drawback is that the
sender must first assure that the public key it is using in the encryption
is actually the receiver's public key.  This is a absolute requirement
in this method.  If the sender cannot authenticate the receiver's public
key this method of private message delivery should not be used.  In SILC
protocol clients can fetch other clients public keys from servers. 
However, the servers may not have authenticated the fetched public key so
that should not be fully trusted.  Use of certificates can solve the
problem.  The receiver's certificate could be authenticated by a third
party Certification Authority (CA).

<br />&nbsp;<br />
Usually verifying the public key is not a problem since the receiver
can provide the public key on some website, or verify the fingerprint of
the key over email, or phone call.  The clients can also fetch the
public keys from SILC servers if they trust that the keys are authentic.
If both of the clients trust that the public keys are authentic using this
method of private message delivery is very simple and recommended.


<br />&nbsp;<br />
<b>Conclusion</b><br />&nbsp;<br />

The Secure Internet Live Conferencing (SILC) protocol is a new generation
chat protocol that provides all the common conferencing services with
strong support for security.  It has wide range of security properties
that should meet the highest levels of security requirements, while not
forgetting easy of use.  The network topology offers new architectural
solution with better scalability over traditional chat protocols.


<br />&nbsp;<br />
<b>Further Information</b><br />&nbsp;<br />

More detailed information about the SILC protocol is available in the
SILC protocol specification documents.  There exists currently four
Internet Drafts that defines the protocol in great detail.  The Internet
Drafts are available from the following sources but also from the
<a href="http://www.ietf.org/" class="normal">IETF website</a>.
<br />&nbsp;<br />

- <a href="docs/draft-riikonen-silc-spec-03.txt" class="normal">
Secure Internet Live Conferencing (SILC), Protocol Specification</a>
<br />
- <a href="docs/draft-riikonen-silc-pp-03.txt" class="normal">
SILC Packet Protocol</a>
<br />
- <a href="docs/draft-riikonen-silc-ke-auth-03.txt" class="normal">
SILC Key Exchange and Authentication Protocols</a>
<br />
- <a href="docs/draft-riikonen-silc-commands-01.txt" class="normal">
SILC Commands</a>
<br />&nbsp;<br />

For comprehensive introduction to cryptography refer to the
<a href="http://www.ssh.com/tech/crypto/" class="normal">Cryptography A-2-Z document</a>.

<br />&nbsp;<br />
<a name="terms"></a>
<b>Terms and Abbreviations</b><br />&nbsp;<br />

- Asymmetric cryptosystem
<br />&nbsp;<br />
Asymmetric cryptosystem provides public encryption.  It has two keys,
one public key and one private key (also called as secret key).  The public
key is publicly available allowing anyone to encrypt messages with the
public key.  Only the posessor of the private key can decrypt those messages.
Difference to symmetric cryptosystem is that symmetric cryptosystem use only
one key, and the key is usually used to both encryption and decryption.  The
asymmetric cryptosystem is also called as public key encryption, public key
cryptosystem or public key algorithm.  SILC supports RSA and DSS asymmetric
cryptosystems.
<br />&nbsp;<br />

- Authentication
<br />&nbsp;<br />
The verification of the identity of a person, host or process in order
to gain access to a service or prove identity.  In data communications
it also means verifying the origin of a message.
<br />&nbsp;<br />

- Certificate
<br />&nbsp;<br />
Certificate is a digital document which can be used to verify the 
identity of a person or host.  In SILC, certificates can be used to prove
identity of clients, servers and routers.  Basically certificate is a public
key with subject name.  SILC supports X.509, OpenPGP and SPKI certificates.
Supported public keys are SILC style public key and SSH2 style public
key.
<br />&nbsp;<br />

- Certification Authority (CA)
<br />&nbsp;<br />
A third party entity that can verify identity of a person or host.  CA
is usually external company that provides certificates and their
verification services.
<br />&nbsp;<br />

- Diffie-Hellman key exchange
<br />&nbsp;<br />
First public key algorithm ever invented.  It is used to generate a secret
key between two or more parties.  It gets its security from the difficulty
of calculating discrete logarithms.
<br />&nbsp;<br />

- Encryption
<br />&nbsp;<br />
A mechanism (usually mathematical) to transfer plaintext (or cleartext)
to ciphertext to provide confidentiality.  A process to transfer
the ciphertext back to plaintext is called decryption.
<br />&nbsp;<br />

- Integrity
<br />&nbsp;<br />
The verification of data to detect any modifications.  If data is
modified enroute from the sender to the receiver, the modification will
be detected.
<br />&nbsp;<br />

- HMAC
<br />&nbsp;<br />
Hash Message Authentication Code.  Also called as keyed hash function.
It is a secret key authentication algorithm which proves that the message
is not modified and that the HMAC was computed by the sender of the
message.
<br />&nbsp;<br />

- Key management
<br />&nbsp;<br />
Key management is a set of processes and mechanisms which support key
exchange and maintainance of current keying relationships between parties,
including replacing older keys with new keys as necessary, by executing
rekey.
<br />&nbsp;<br />

- Man-in-the-middle attack
<br />&nbsp;<br />
An attack against two connecting entities where the attacker executes
key exchange protocol with both of the parties indepently without
their knowledge.  Both of the connecting entities will end up having secret
key with the attacker, and the attacker can encrypt and decrypt all the
messages that goes between the two entities.
<br />&nbsp;<br />

- Message Authentication Code (MAC)
<br />&nbsp;<br />
MAC provides message integrity by computing the MAC using a secret
key authentication algorithm (HMAC).
<br />&nbsp;<br />

- Perfect Forward Secrecy (PFS)
<br />&nbsp;<br />
A property of rekey (or key regeneration) which defines whether the
new key is derived from the old key.  If Perfect Forward Secrecy is
selected the new key is never dependent of the old key which means
that if the old key would get compromised at later time it will not
compromise the new key.  In SILC setting PFS in the SKE protocol means
executing the SKE protocol again.  If PFS is not selected the new key
is always derived from the old key.
<br />&nbsp;<br />

- Rekey
<br />&nbsp;<br />
A key regeneration process where the old key has expired or is not
secure anymore to use.  In this case rekey is performed and new key
is generated.
<br />&nbsp;<br />

- Symmetric cryptosystem
<br />&nbsp;<br />
Symmetric cryptosystem is one key cryptosystem where one key is used
usually to both encryption and decryption process.  The symmetric
cryptosystems are usually significantly faster than asymmetric cryptosystems.
DES, AES, Twofish and Blowfish are examples of symmetric cryptosystems.
SILC supports all the common symmetric cryptosystems including AES.
SILC does not support DES as it is insecure and 3DES as it is too slow.