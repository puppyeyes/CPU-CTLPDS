package jsonGenerate;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.LinkedList;
import java.util.List;

import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import ABPDS.Configuration;
import ABPDS.PDS;
import ABPDS.Rule;

public class GenerateXML {

	public static void main(String[] args) throws Exception {
		PDS abpds = new PDS();
		List<Rule> rule_list = new LinkedList<Rule>();
		List<String> final_state = new LinkedList<String>();
		List<String> state_list = new LinkedList<>();
		List<String> stack_list = new LinkedList<>();
		try { // ��ֹ�ļ��������ȡʧ�ܣ���catch��׽���󲢴�ӡ��Ҳ����throw

			/* ����TXT�ļ� */
			String pathname = "/home/chuancy/git/GPU-CTLPDS/PuMoC/abpds.txt"; // ����·�������·�������ԣ������Ǿ���·����д���ļ�ʱ��ʾ���·��
			File filename = new File(pathname); // Ҫ��ȡ����·����input��txt�ļ�
			InputStreamReader reader = new InputStreamReader(new FileInputStream(filename)); // ����һ������������reader
			BufferedReader br = new BufferedReader(reader); // ����һ�����������ļ�����ת�ɼ�����ܶ���������
			String line = "";
			line = br.readLine();

			while (line != null) {
				

				if (line != null && !line.startsWith("FinalState:")) {

					List<Configuration> to_list = new LinkedList<Configuration>();
					line = line.trim();
					String[] configure_string = line.split("-->");
					String[] from_string = configure_string[0].trim().substring(1, configure_string[0].length() - 2)
							.split(",");
					if(from_string.length>2)
					{
						from_string[0]=from_string[0]+','+from_string[1]+','+from_string[2];
					}
					Configuration from = new Configuration(from_string[0], from_string[from_string.length-1]);
					if (!state_list.contains(from_string[0])) {
						state_list.add(from_string[0]);
					}
					if (!stack_list.contains(from_string[from_string.length-1])) {
						stack_list.add(from_string[from_string.length-1]);
						//System.out.println(from_string[1]);
					}

					String[] to_configure_string = configure_string[1].trim()
							.substring(2, configure_string[1].length() - 3).split("><");
					for (int i = 0; i < to_configure_string.length; i++) {
						String[] to_string = to_configure_string[i].split(",");
						String[] stack_string = to_string[to_string.length-1].split(" ");
						if(to_string.length>2)
						{
							to_string[0]=to_string[0]+','+to_string[1]+','+to_string[2];	
						}
						Configuration to;
						if (stack_string.length == 1) {
							to = new Configuration(to_string[0].trim(), stack_string[0].trim());
							if (!state_list.contains(to_string[0].trim())) {
								state_list.add(to_string[0].trim());
							}
							if (!stack_list.contains(stack_string[0].trim())) {
								if (!stack_string[0].trim().equals("*")) {
									stack_list.add(stack_string[0].trim());
									//System.out.println(stack_string[0].trim());
								}
							}
						} else {
							to = new Configuration(to_string[0].trim(), stack_string[0].trim(), stack_string[1].trim());
							if (!state_list.contains(to_string[0].trim())) {
								state_list.add(to_string[0].trim());
							}
							if (!stack_list.contains(stack_string[0].trim())) {
								if (!stack_string[0].trim().equals("*")) {
									stack_list.add(stack_string[0].trim());
									//System.out.println(stack_string[0].trim());
								}
							}
							if (!stack_list.contains(stack_string[stack_string.length-1].trim())) {
								stack_list.add(stack_string[stack_string.length-1].trim());
								//System.out.println(stack_string[1].trim());
							}
						}
						to_list.add(to);
					}
					Rule rule = new Rule(from, to_list);
					rule_list.add(rule);
				} else if (line != null) {
					String final_state_string = line.substring(11, line.length() - 1);
					String[] state = final_state_string.split(";");
					for (int i = 0; i < state.length; i++) {
						//System.out.println(state[i]);
						if(!final_state.contains(state[i]))
						{
							final_state.add(state[i]);
						}
						if (!state_list.contains(state[i])) {
							state_list.add(state[i]);
						}
					}
				}
				// System.out.println(br.readLine());
				line = br.readLine(); // һ�ζ���һ������
			}
			abpds.setFinalState(final_state);
			abpds.setRulelist(rule_list);
			abpds.setStateSize(state_list.size());
			abpds.setStackSize(stack_list.size());
			abpds.setRuleSize(rule_list.size());
//			for(String s:stack_list)
//			{
//				System.out.println(s);
//			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		createXMLDemo(abpds);
		System.out.println("转换结束");
	}

	public static void createXMLDemo(PDS abpds) throws Exception {
		Document document = DocumentHelper.createDocument();
		Element root = document.addElement("abpds");
		root.addAttribute("state_size", String.valueOf(abpds.getStateSize()));
		root.addAttribute("stack_size", String.valueOf(abpds.getStackSize()));
		root.addAttribute("rule_size", String.valueOf(abpds.getRuleSize()));
		Element finalStateElement = root.addElement("finalState").addAttribute("size",
				String.valueOf(abpds.getFinalState().size()));
		Element deltaElement = root.addElement("delta").addAttribute("size",
				String.valueOf(abpds.getRulelist().size()));

		for (int i = 0; i < abpds.getFinalState().size(); i++) {
			finalStateElement.addElement("state").addText(abpds.getFinalState().get(i));
		}

		for (int i = 0; i < abpds.getRulelist().size(); i++) {
			int toSize = abpds.getRulelist().get(i).getTo().size();
			Element ruleElement = deltaElement.addElement("rule").addAttribute("toSize", String.valueOf(toSize));
			Element fromElement = ruleElement.addElement("from");
			fromElement.addAttribute("controlLocation", abpds.getRulelist().get(i).getFrom().controlLocation);
			fromElement.addAttribute("stack", abpds.getRulelist().get(i).getFrom().stack1);
			for (int j = 0; j < toSize; j++) {
				Element toElement = ruleElement.addElement("to");
				toElement.addAttribute("controlLocation", abpds.getRulelist().get(i).getTo().get(j).controlLocation);
				toElement.addAttribute("stack1", abpds.getRulelist().get(i).getTo().get(j).stack1);
				toElement.addAttribute("stack2", abpds.getRulelist().get(i).getTo().get(j).stack2);
			}
		}
		// д�뵽һ���µ��ļ���
		writer(document);
	}

	/**
	 * ��document����д���µ��ļ�
	 * 
	 * @param document
	 * @throws Exception
	 */
	public static void writer(Document document) throws Exception {
		// ���յĸ�ʽ
		// OutputFormat format = OutputFormat.createCompactFormat();
		// �Ű������ĸ�ʽ
		OutputFormat format = OutputFormat.createPrettyPrint();
		// ���ñ���
		format.setEncoding("UTF-8");
		// ����XMLWriter����,ָ����д���ļ��������ʽ
		// XMLWriter writer = new XMLWriter(new FileWriter(new
		// File("src//a.xml")),format);
		XMLWriter writer = new XMLWriter(new OutputStreamWriter(new FileOutputStream(new File("abpds.xml")), "UTF-8"),
				format);
		// д��
		writer.write(document);
		// ����д��
		writer.flush();
		// �رղ���
		writer.close();
	}
}
