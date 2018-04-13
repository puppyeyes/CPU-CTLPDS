package jsonGenerate;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.lang.reflect.AnnotatedParameterizedType;
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.List;

import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

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
		try { // 防止文件建立或读取失败，用catch捕捉错误并打印，也可以throw

			/* 读入TXT文件 */
			String pathname = "abpds.txt"; // 绝对路径或相对路径都可以，这里是绝对路径，写入文件时演示相对路径
			File filename = new File(pathname); // 要读取以上路径的input。txt文件
			InputStreamReader reader = new InputStreamReader(new FileInputStream(filename)); // 建立一个输入流对象reader
			BufferedReader br = new BufferedReader(reader); // 建立一个对象，它把文件内容转成计算机能读懂的语言
			String line = "";
			line = br.readLine();

			while (line != null) {
				line = br.readLine(); // 一次读入一行数据

				if (line != null && !line.startsWith("FinalState:")) {

					List<Configuration> to_list = new LinkedList<Configuration>();
					line = line.trim();
					String[] configure_string = line.split("-->");
					String[] from_string = configure_string[0].trim().substring(1, configure_string[0].length() - 2)
							.split(",");

					Configuration from = new Configuration(from_string[0], from_string[1]);
					if (!state_list.contains(from_string[0])) {
						state_list.add(from_string[0]);
					}
					if (!stack_list.contains(from_string[1])) {
						stack_list.add(from_string[1]);
						//System.out.println(from_string[1]);
					}

					String[] to_configure_string = configure_string[1].trim()
							.substring(2, configure_string[1].length() - 3).split("><");
					for (int i = 0; i < to_configure_string.length; i++) {
						String[] to_string = to_configure_string[i].split(",");
						String[] stack_string = to_string[1].split(" ");
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
							if (!stack_list.contains(stack_string[1].trim())) {
								stack_list.add(stack_string[1].trim());
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
						final_state.add(state[i]);
						if (!state_list.contains(state[i])) {
							state_list.add(state[i]);
						}
					}
				}
				// System.out.println(br.readLine());
			}
			abpds.setFinalState(final_state);
			abpds.setRulelist(rule_list);
			abpds.setStateSize(state_list.size());
			abpds.setStackSize(stack_list.size());
//			for(String s:stack_list)
//			{
//				System.out.println(s);
//			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		createXMLDemo(abpds);
	}

	public static void createXMLDemo(PDS abpds) throws Exception {
		Document document = DocumentHelper.createDocument();
		Element root = document.addElement("abpds");
		root.addAttribute("state_size", String.valueOf(abpds.getStateSize()));
		root.addAttribute("stack_size", String.valueOf(abpds.getStackSize()));
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
		// 写入到一个新的文件中
		writer(document);
	}

	/**
	 * 把document对象写入新的文件
	 * 
	 * @param document
	 * @throws Exception
	 */
	public static void writer(Document document) throws Exception {
		// 紧凑的格式
		// OutputFormat format = OutputFormat.createCompactFormat();
		// 排版缩进的格式
		OutputFormat format = OutputFormat.createPrettyPrint();
		// 设置编码
		format.setEncoding("UTF-8");
		// 创建XMLWriter对象,指定了写出文件及编码格式
		// XMLWriter writer = new XMLWriter(new FileWriter(new
		// File("src//a.xml")),format);
		XMLWriter writer = new XMLWriter(new OutputStreamWriter(new FileOutputStream(new File("abpds.xml")), "UTF-8"),
				format);
		// 写入
		writer.write(document);
		// 立即写入
		writer.flush();
		// 关闭操作
		writer.close();
	}
}
