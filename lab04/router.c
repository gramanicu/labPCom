#include "skel.h"

int interfaces[ROUTER_NUM_INTERFACES];
struct route_table_entry *rtable;
int rtable_size;

struct arp_entry *arp_table;
int arp_table_len;

int getMask(struct route_table_entry elem, __u32 dest_ip){
	return (elem.mask &dest_ip) != elem.prefix ? 0 : elem.mask;
}

/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/
struct route_table_entry *get_best_route(__u32 dest_ip) {
    struct route_table_entry* elem = NULL;
    int maxMask = 0;

    for(int i=0;i<rtable_size;i++){
        int currentMask = getMask(rtable[i],dest_ip);
        if(currentMask > maxMask){
            maxMask = currentMask;
            elem = (rtable+i);
        }
    }
    return elem;
}

/*
 Returns a pointer (eg. &arp_table[i]) to the best matching ARP entry.
 for the given dest_ip or NULL if there is no matching entry.
*/
struct arp_entry *get_arp_entry(__u32 ip) {
    /* TODO 2: Implement */
	for(int i = 0; i < arp_table_len; ++i) {
		if(arp_table[i].ip == ip) {
			return (arp_table + i);
		}
	}
    return NULL;
}

int main(int argc, char *argv[])
{
	msg m;
	int rc;

	init();
	for(int i = 0; i < 4; i++) {
		printf("%d\n", interfaces[i]);	
	}
	rtable = malloc(sizeof(struct route_table_entry) * 100);
	arp_table = malloc(sizeof(struct  arp_entry) * 100);
	DIE(rtable == NULL, "memory");
	rtable_size = read_rtable(rtable);
	parse_arp_table();
	/* Students will write code here */

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;
printf("%d\n", eth_hdr->ether_type);
        if (eth_hdr->ether_type == ETHERTYPE_ARP) {
            printf("IT'S A FUCKING ARP\n");
        } else if(eth_hdr->ether_type == ETHERTYPE_IP){
            printf("IT'S A FUCKING IP\n");
        } else {
            printf("I DON'T KNOW WHAT THE FUCK THIS IS\n");
        }

		
	}
}
