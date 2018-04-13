#include "utility.cuh"

#define DEFAULT_XML_FILE "abpds.xml"
using namespace std;
int main() {
	char * file_name = DEFAULT_XML_FILE;
	if (parse_abpds_xml(file_name) != 0) {
		printf("Failed to parse abpds ");
	} else {
		printf("parse abpds compelet");
	}
	return 0;
}
