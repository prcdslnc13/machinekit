//   client resolve API test program using ll_zeroconf_resolve();
//
//   Michael Haberler, 2014
//
//   avahi is free software; you can redistribute it and/or modify it
//   under the terms of the GNU Lesser General Public License as
//   published by the Free Software Foundation; either version 2.1 of the
//   License, or (at your option) any later version.
//
//   avahi is distributed in the hope that it will be useful, but WITHOUT
//   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
//   Public License for more details.
//
//   You should have received a copy of the GNU Lesser General Public
//   License along with avahi; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <net/if.h>


#include "config.h"
#include "ll-zeroconf.hh"
#include "syslog_async.h"

int main(AVAHI_GCC_UNUSED int argc, char*argv[])
{
    openlog_async(argv[0], LOG_NDELAY|LOG_ERR, LOG_LOCAL1);
    setlogmask_async(LOG_UPTO(LOG_DEBUG));

    if (argc < 2) {
	fprintf(stderr, "usage: zres subtype [uuid=<uuid>]\n");
	exit(1);
    }
    char subtype[100];
    snprintf(subtype, sizeof(subtype), "_%s._sub._machinekit._tcp", argv[1]);

    zresolve_t res = {0};
    res.proto =	 AVAHI_PROTO_UNSPEC;
    res.interface = AVAHI_IF_UNSPEC;
    res.type = subtype;
    if (argc > 2)
	res.match =  argv[2];
    res.domain = NULL;
    res.name = NULL;
    res.timeout_ms = 3000;

    resolve_context_t *p  = ll_zeroconf_resolve(&res);

    fprintf(stderr, "result = %d\n",res.result);
    if (res.result == SD_OK) {
	fprintf(stderr, "name='%s'\n",res.name);

	char a[AVAHI_ADDRESS_STR_MAX], *t;
	avahi_address_snprint(a, sizeof(a), &res.address);
	fprintf(stderr, "address='%s'\n",a);

	fprintf(stderr, "ntxt=%d\n", avahi_string_list_length(res.txt));

	t = avahi_string_list_to_string(res.txt);
	fprintf(stderr, "txt=%s\n", t);
	avahi_free(t);

	fprintf(stderr, "domain=%s\n", res.domain);
	fprintf(stderr, "interface=%d\n", res.interface);
	fprintf(stderr, "proto=%d\n", res.proto);
	fprintf(stderr, "port=%d\n", res.port);
	fprintf(stderr, "flags=0x%x\n", res.flags);
    }
    ll_zeroconf_resolve_free(p);

}
