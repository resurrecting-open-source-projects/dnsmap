# dnsmap

#### dnsmap - scan for subdomains using brute-forcing techniques

## Help this project ##

dnsmap needs your help. **If you are a programmer** and if you want to help a
nice project, this is your opportunity.

dnsmap was imported from some tarballs (the original homepage[1] and
developers are inactive). After this, all patches found in Debian project and
other places for this program were applied. All initial work was registered in
ChangeLog file (version 1.6 and later releases). dnsmap is being packaged
in Debian[2] Project.

If you are interested to help dnsmap, read the [CONTRIBUTING.md](CONTRIBUTING.md) file.

[1]: https://code.google.com/archive/p/dnsmap/
[2]: https://tracker.debian.org/pkg/dnsmap

## What is dnsmap? ##

dnsmap scans a domain for common subdomains using a built-in or an external
wordlist (if specified using -w option). The internal wordlist has around 1000
words in English and Spanish as ns1, firewall servicios and smtp. So will be
possible search for smtp.example.com inside example.com automatically. Results
can be saved in CSV and human-readable format for further processing. dnsmap
does NOT require root privileges to be run, and should NOT be run with such
privileges for security reasons.

dnsmap was originally released back in 2006 and was inspired by the fictional
story "The Thief No One Saw" by Paul Craig, which can be found in the book
"Stealing the Network - How to 0wn the Box".

dnsmap is mainly meant to be used by pentesters during the information
gathering/enumeration phase of infrastructure security assessments. During the
enumeration stage, the security consultant would typically discover the target
company's IP netblocks, domain names, phone numbers, etc.

Subdomain brute-forcing is another technique that should be used in the
enumeration stage, as it's especially useful when other domain enumeration
techniques such as zone transfers don't work (I rarely see zone transfers being
publicly allowed these days by the way).

Fun things that can happen:

 1. Finding interesting remote access servers (e.g.:
    https:<span>://</span>extranet.example.com).

 2. Finding badly configured and/or unpatched servers (e.g.: test.example.com).

 3. Finding new domain names which will allow you to map
    non-obvious/hard-to-find netblocks of your target organization (registry
    lookups - aka whois is your friend).

 4. Sometimes you find that some bruteforced subdomains resolve to internal IP
    addresses (RFC 1918).     This is great as sometimes they are real
    up-to-date "A" records which means that it *is* possible to enumerate
    internal servers of a target organization from the Internet by only using
    standard DNS resolving (as opposed to zone transfers for instance).

 5. Discover embedded devices configured using Dynamic DNS services (e.g.: IP
    Cameras). This method     is an alternative to finding devices via Google
    hacking techniques.

## Build and Install ##

To build and install, run the following commands:

    $ ./autogen.sh
    $ ./configure
    $ make
    # make install

To return to original source code you can use '$ make distclean' command.

On Debian systems you can use '# apt install dnsmap'.

## Author ##

dnsmap was originally developed by "pagvac" under GPL-2+ license.

Currently, the source code and newer versions are available at
https://github.com/resurrecting-open-source-projects/dnsmap
