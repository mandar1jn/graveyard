using System.CommandLine;

namespace Brainfuck
{
	internal class Program
	{
		const string allowedChars = "><=+-.,[]";

		static async Task<int> Main(string[] args)
		{
			var fileArgument = new Argument<FileInfo>(name: "file", description: "The brainfuck file to interpret");
			var memoryOption = new Option<int>(name: "memory", description: "The amount of memory alloted to the program", getDefaultValue: () => 30000);

			var rootCommand = new RootCommand("Simple brainfuck interpreter")
			{
				fileArgument,
				memoryOption
			};

			rootCommand.SetHandler((file, memorySize) =>
			{
				if (file is not null && file.Exists)
				{
					string code = "";
					using (StreamReader sr = file.OpenText())
					{
						string? line = "";
						while ((line = sr.ReadLine()) != null)
						{
							code += line;
						}
					}
					string instructions = new(code.Where(ch => allowedChars.Contains(ch)).ToArray());

					byte[] memory = new byte[memorySize];
					int memPtr = 0;

					for (int i = 0; i < instructions.Length; i++)
					{
						switch (instructions[i])
						{
							case '>':
								memPtr++;
								break;
							case '<':
								memPtr--;
								break;
							case '+':
								memory[memPtr]++;
								break;
							case '-':
								memory[memPtr]--;
								break;
							case '.':
								if(memory[memPtr] == 10)
								{
									Console.Write(Environment.NewLine);
									break;
								}
								Console.Write(Convert.ToChar(memory[memPtr]));
								break;
							case ',':
								Console.WriteLine("Enter a value: ");
								char ch = Convert.ToChar(Console.Read());
								Console.WriteLine(ch);
								memory[memPtr] = Convert.ToByte(ch);
								break;
							case '[':
								if (memory[memPtr] == 0)
								{
									int skip = 0;
									i++;
									while (instructions[i] != ']' || skip > 0)
									{
										if (instructions[i] == '[')
										{
											skip += 1;
										}
										else if (instructions[i] == ']')
										{
											skip -= 1;
										}
										i++;
									}
								}
								break;
							case ']':
								if(memory[memPtr] != 0)
								{
									int skip = 0;
									i--;
									while (instructions[i] != '[' || skip > 0)
									{
										if (instructions[i] == ']')
										{
											skip += 1;
										}
										else if (instructions[i] == '[')
										{
											skip -= 1;
										}
										i--;
									}
								}
								break;
						}
					}


				}
				else
				{
					Console.WriteLine("Please enter a valid file path");
				}
			}, fileArgument, memoryOption);


			return await rootCommand.InvokeAsync(args);
		}
	}
}