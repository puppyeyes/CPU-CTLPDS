import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;

public class BMGenerate {

	public static void main(String[] args) throws IOException {
		System.out.println("输入state值 stack值 rule的数量");
		// TODO Auto-generated method stub
		String name = "bm.abpds";
		Scanner sb = new Scanner(System.in);
		int state_num = Integer.valueOf(sb.nextLine());
		int stack_num = Integer.valueOf(sb.nextLine());
		int rule_num = Integer.valueOf(sb.nextLine());
		Set<String> rule_set = new HashSet<String>();
		File file = new File(name);
		if (!file.exists()) {
			file.createNewFile();
		} else {

			file.delete();// 删除文件
			file.createNewFile();
		}

		// write
		FileWriter fw = new FileWriter(file, true);
		BufferedWriter bw = new BufferedWriter(fw);
		bw.write("(p0 <r0 r3>)\n");
		bw.write("ACCEPTING  p1\n");
		int count = 0;
		Random random = new Random();

		while (true) {
			Rule rule = new Rule();
			int state = random.nextInt(state_num);
			int stack = random.nextInt(stack_num);
			FromConfig from = new FromConfig("p" + String.valueOf(state), "r" + String.valueOf(stack));
			rule.setFrom(from);
			int type = random.nextInt(25);
			if (type < 21) {
				while (true) {
					state = random.nextInt(state_num);
					stack = random.nextInt(stack_num);
					ToConfig to = new ToConfig("p" + String.valueOf(state), "r" + String.valueOf(stack));
					rule.setTo(to);
					if (!to.toString().equals(from.toString())) {
						break;
					}
				}
			} else if (type >= 21 && type <= 25) {
				state = random.nextInt(state_num);
				ToConfig to = new ToConfig("p" + String.valueOf(state), " ");
				rule.setTo(to);
			} else if (type > 25 && type <= 28) {
				state = random.nextInt(state_num);
				stack = random.nextInt(stack_num);
				int stack2 = random.nextInt(stack_num);
				ToConfig to = new ToConfig("p" + String.valueOf(state), "r" + String.valueOf(stack),
						"r" + String.valueOf(stack2));
				rule.setTo(to);
			} else if (type == 29) {
				int size = random.nextInt(10) + 2;
				Set<String> toConfig_set = new HashSet<String>();
				ToConfig tmp;
				ToConfig to;
				ToConfig sub;
				while (true) {
					state = random.nextInt(state_num);
					stack = random.nextInt(stack_num);
					to = new ToConfig("p" + String.valueOf(state), "r" + String.valueOf(stack));
					toConfig_set.add(to.toString());
					tmp = to;
					if (!to.toString().equals(from.toString())) {
						break;
					}
				}
				for (int i = 0; i < size;) {
					while (true) {
						state = random.nextInt(state_num);
						stack = random.nextInt(stack_num);
						sub = new ToConfig("p" + String.valueOf(state), "r" + String.valueOf(stack));
						if (!sub.toString().equals(from.toString())) {
							break;
						}
					}
					if (!toConfig_set.contains(sub.toString())) {
						toConfig_set.add(sub.toString());
						tmp.next = sub;
						tmp = sub;
						i++;
					}
				}
				rule.setTo(to);
			}
			String rule_string = rule.toString();
			if (!rule_set.contains(rule_string)) {
				rule_set.add(rule_string);
				bw.write(rule_string);
				count++;
			}

			if (count == rule_num) {
				break;
			}
		}

		bw.flush();
		bw.close();
		fw.close();
	}

}
