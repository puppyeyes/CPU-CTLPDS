#include "utility.cuh"
#include "abpds.cuh"
map<string, int> state_mp;
map<string, int> stack_mp;
int *finalStateArray;
static int parse_abpds(xmlDocPtr doc, xmlNodePtr cur) {
	int state_count = 0; //记录state的个数
	int stack_count = 0; //记录state的个数
	/*建立一個迭代器查找MAP*/
	map<string, int>::iterator it_find;

	assert(doc || cur);
	int finalStateSize;
	/*
	 * 初始化映射stack * 爲0
	 */
	stack_mp.insert(pair<string, int>("*", stack_count));
	stack_count++;

	/*统计delta的大小*/
	int delta_size;
	if (!xmlStrcmp(cur->name, (const xmlChar *) "abpds")) {
		int state_size = atoi((char *) xmlGetProp(cur, BAD_CAST "state_size"));
		int stack_size = atoi((char *) xmlGetProp(cur, BAD_CAST "stack_size"));
		delta_size = state_size * state_size;
	}
	initDelta(delta_size, delta);
	/*
	 * 计算finalStateArray的存放位置
	 * */
	int finalStateCount = 0;
	/*临时在cpu中存放rule方便处理*/
	TransitionRule *transitionRuleList;
	//printf("name %s\n", cur->name);
	cur = cur->xmlChildrenNode;
	string tmp;
	while (cur != NULL) {
		//获取final state
		if (!xmlStrcmp(cur->name, (const xmlChar *) "finalState")) {
			printf("parse finalState\n");

			finalStateSize = atoi((char *) xmlGetProp(cur, BAD_CAST "size"));
			printf("final state size: %d\n", finalStateSize);
			cudaMallocManaged(&finalStateArray, sizeof(int) * finalStateSize);
			xmlNodePtr stateCur = cur->xmlChildrenNode;
			while (stateCur != NULL) {
				if (!xmlStrcmp(stateCur->name, (const xmlChar *) "state")) {
					tmp = (char*) xmlNodeGetContent(stateCur);
					if (!state_mp.count(tmp)) {
						state_mp.insert(pair<string, int>(tmp, state_count));
						finalStateArray[finalStateCount] = state_count;
						finalStateCount++;
						state_count++;
						cout << tmp << endl;
					}
				}
				stateCur = stateCur->next;
			}
		}
		//获取transition rule
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "delta"))) {
			printf("parse delta\n");
			delta_size = atoi((char *) xmlGetProp(cur, BAD_CAST "size"));
			xmlNodePtr ruleCur = cur->xmlChildrenNode;
			while (ruleCur != NULL) {
				if ((!xmlStrcmp(ruleCur->name, (const xmlChar *) "rule"))) {
					int to_config_size = atoi(
							(char *) xmlGetProp(ruleCur, BAD_CAST "toSize"));

					TransitionRule r;
					xmlNodePtr configCur = ruleCur->xmlChildrenNode;
					while (configCur != NULL) {
						if ((!xmlStrcmp(configCur->name, BAD_CAST "from"))) {
							string from_control_location = (char *) xmlGetProp(
									configCur,
									BAD_CAST "controlLocation");
							cout << from_control_location;
							if (!state_mp.count(from_control_location)) {
								state_mp.insert(
										pair<string, int>(from_control_location,
												state_count));
								r.from.controlLocation = state_count;
								state_count++;
							} else {
								it_find = state_mp.find(from_control_location);
								if (it_find != state_mp.end()) {
									r.from.controlLocation = it_find->second;
								} else {
									printf("xml parse error:inter error 1\n");
								}
							}
							string from_stack = (char *) xmlGetProp(configCur,
							BAD_CAST "stack");
							cout << "," << from_stack << "-->";
							if (from_stack.compare("*") == 0) {
								printf("xml parse error:syntax error\n");
							}
							if (!stack_mp.count(from_stack)) {
								stack_mp.insert(
										pair<string, int>(from_stack,
												stack_count));
								r.from.stack = stack_count;
								stack_count++;
							} else {
								it_find = stack_mp.find(from_stack);
								if (it_find != stack_mp.end()) {
									r.from.stack = it_find->second;
								} else {
									printf("xml parse error:inter error 2\n");
								}
							}
						}

						if (to_config_size == 1) {
							r.tag = false;
							if ((!xmlStrcmp(configCur->name,
									(const xmlChar *) "to"))) {
								string to_control_location =
										(char *) xmlGetProp(configCur,
										BAD_CAST "controlLocation");
								cout << to_control_location;
								r.to = (ToConfig *) malloc(sizeof(ToConfig));
								if (!state_mp.count(to_control_location)) {
									state_mp.insert(
											pair<string, int>(
													to_control_location,
													state_count));
									r.to[0].controlLocation = state_count;
									state_count++;
								} else {
									it_find = state_mp.find(
											to_control_location);
									if (it_find != state_mp.end()) {
										r.to[0].controlLocation =
												it_find->second;
									} else {
										printf(
												"xml parse error:inter error 3\n");
									}
								}
								string to_control_stack_1 = (char *) xmlGetProp(
										configCur,
										BAD_CAST "stack1");
								cout << "," << to_control_stack_1;
								if (!stack_mp.count(to_control_stack_1)) {
									stack_mp.insert(
											pair<string, int>(
													to_control_stack_1,
													stack_count));
									r.to[0].stack1 = stack_count;
									stack_count++;
								} else {
									it_find = stack_mp.find(to_control_stack_1);
									if (it_find != stack_mp.end()) {
										r.to[0].stack1 = it_find->second;
									} else {
										printf(
												"xml parse error:inter error 4\n");
									}
								}
								if ((char *) xmlGetProp(configCur,
								BAD_CAST "stack2") != NULL) {
									string to_control_stack_2 =
											(char *) xmlGetProp(configCur,
											BAD_CAST "stack2");
									cout << " " << to_control_stack_2 << endl;
									if (!stack_mp.count(to_control_stack_2)) {
										stack_mp.insert(
												pair<string, int>(
														to_control_stack_2,
														stack_count));
										r.to[0].stack2 = stack_count;
										stack_count++;
									} else {
										it_find = stack_mp.find(
												to_control_stack_2);
										if (it_find != stack_mp.end()) {
											r.to[0].stack2 = it_find->second;
										} else {
											printf(
													"xml parse error:inter error 5\n");
										}
									}
								} else {
									cout << endl;
								}

							}
						} else {
							r.tag = true;
							if ((!xmlStrcmp(configCur->name,
									(const xmlChar *) "to"))) {
								r.to = (ToConfig *) malloc(
										sizeof(ToConfig) * to_config_size);
								int i = 0;
								while (configCur != NULL) {
									if ((!xmlStrcmp(configCur->name,
											(const xmlChar *) "to"))) {
										string to_control_location =
												(char *) xmlGetProp(configCur,
												BAD_CAST "controlLocation");
										cout << to_control_location;
										if (!state_mp.count(
												to_control_location)) {
											state_mp.insert(
													pair<string, int>(
															to_control_location,
															state_count));
											r.to[i].controlLocation =
													state_count;
											state_count++;
										} else {
											it_find = state_mp.find(
													to_control_location);
											if (it_find != state_mp.end()) {
												r.to[i].controlLocation =
														it_find->second;
											} else {
												printf(
														"xml parse error:inter error 6\n");
											}
										}
										string to_control_stack_1 =
												(char *) xmlGetProp(configCur,
												BAD_CAST "stack1");
										cout << "," << to_control_stack_1<<"&";
										if (!stack_mp.count(
												to_control_stack_1)) {
											stack_mp.insert(
													pair<string, int>(
															to_control_stack_1,
															stack_count));
											r.to[0].stack1 = stack_count;
											stack_count++;
										} else {
											it_find = stack_mp.find(
													to_control_stack_1);
											if (it_find != stack_mp.end()) {
												r.to[0].stack1 =
														it_find->second;
											} else {
												printf(
														"xml parse error:inter error 7\n");
											}
										}
									}
									configCur = configCur->next;
									i++;
								}
								cout<<endl;
								continue;
							}
						}
						configCur = configCur->next;
					}

				}
				ruleCur = ruleCur->next;
			}

		}
		cur = cur->next;
	}
	return 0;
}

int parse_abpds_xml(const char *file_name) {

	assert(file_name);

	xmlDocPtr doc;   //xml整个文档的树形结构
	xmlNodePtr cur;   //xml节点

//获取树形结构
	doc = xmlParseFile(file_name);
	if (doc == NULL) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

//获取根节点
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	if ((xmlStrcmp(cur->name, (const xmlChar *) "abpds"))) {
		fprintf(stderr, "The root is not abpds.\n");
		goto FAILED;
	}

//遍历处理根节点的每一个子节点
//cur = cur->xmlChildrenNode;
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "abpds"))) {
//        id = xmlGetProp(cur, "id");
//        printf("id:%s\t",id);
		parse_abpds(doc, cur);
	}
	xmlFreeDoc(doc);
	return 0;
	FAILED: if (doc) {
		xmlFreeDoc(doc);
	}
	return -1;
}
