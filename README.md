<h1>Minishell</h1>

<h2>Introduction</h2>
<p>Minishell is a simple, minimalist shell program designed to mimic the functionality of a Unix-like command-line interpreter. It serves as an educational tool for understanding the basic mechanisms of a shell, such as parsing commands, executing programs, and handling input/output redirection.</p>

<h2>Features</h2>
<ul>
    <li>Basic command execution (e.g., <code>ls</code>, <code>echo</code>, <code>cat</code>).</li>
    <li>Built-in commands <code>cd</code>, <code>exit</code>,  <code>export</code>, <code>unset</code>, <code>echo</code>, and <code>pwd</code>.</li>
    <li>Input and output redirection (<code><</code>, <code>></code>, <code><<</code>, <code>>></code>).</li>
    <li>Pipe handling (<code>|</code>) for chaining commands.</li>
    <li>Environment variable handling.</li>
    <li>Logical operators (<code>&&</code> and <code>||</code>)</li>
    <li>Wildcars (<code>*</code>).</li>
    <li>Signal handling (e.g., <code>Ctrl+C</code> to terminate a running process).</li>
</ul>

<h2>Installation</h2>
<p>To install Minishell, clone the repository and compile the source code using a C compiler like <code>gcc</code>.</p>
<pre><code>git clone https://github.com/aaltinto/minishell.git minishell
cd minishell
make bonus
</code></pre>

<h2>Usage</h2>
<p>Run the compiled Minishell executable:</p>
<pre><code>./minishell
</code></pre>
<p>Once running, you can use it like a standard shell:</p>
<pre><code>$ pwd
/home/user/minishell
$ ls -l
total 64
-rw-r--r-- 1 user user 12345 Jan  1 12:34 main.c
$ echo "Hello, World!"
Hello, World!
$ cd ..
$ exit
</code></pre>

<h2>Commands</h2>

<h3>Built-in Commands</h3>
<ul>
    <li><code>cd [directory]</code>: Change the current directory.</li>
    <li><code>exit [status]</code>: Exit the shell with an optional status code.</li>
    <li><code>pwd</code>: Print the current working directory.</li>
    <li><code>export [variable]</code>: Add new variable in to env.</li>
    <li><code>unset [variable]</code>: Delete variables from env.</li>
    <li><code>echo [message]</code>: Print messages.</li>
</ul>

<h3>External Commands</h3>
<p>Any executable file found in the directories listed in the <code>PATH</code> environment variable can be executed.</p>

<h3>Redirection</h3>
<ul>
    <li><code>command &gt; file</code>: Redirect standard output to a file.</li>
    <li><code>command &lt; file</code>: Redirect standard input from a file.</li>
    <li><code>command &gt;&gt; file</code>: Append standard output to a file.</li>
    <li><code>command &lt;&lt; [delimiter]</code>: Redirect multiple lines of standard input from shell.</li>
</ul>

<h3>Pipes</h3>
<p><code>command1 | command2</code>: Use the output of <code>command1</code> as the input to <code>command2</code>.</p>

<h2>Environment Variables</h2>
<p>Minishell supports setting and using environment variables.</p>
<pre><code>$ export VAR=value
$ echo $VAR
value
</code></pre>

<h2>Logical Operators</h2>
<p>Minishell supports logical operators .</p>
<pre><code>$ mkdir new && echo folder succesfully created
folder succesfully created
$ ls -z || echo ls failed to execute
ls failed to execute
</code></pre>

<h2>Wildcards</h2>
<p>Minishell can search for the files by given letters.</p>
<pre><code>$ echo *.txt
/* All files with .txt extentions */
</code></pre>

<h2>Signal Handling</h2>
<p>Minishell handles common signals like <code>SIGINT</code> (<code>Ctrl+C</code>) to interrupt running processes.</p>

<h2>Acknowledgments</h2>
<p>Minishell was inspired by various Unix-like shells and is developed for educational purposes. Special thanks to all contributors and open-source projects that make learning and development enjoyable.</p>

<h2>Contact</h2>
<p>For questions or support, open an issue in the repository or contact the maintainer at <a href="mailto:ali.altintoprak@outlook.com">ali.altintoprak@outlook.com</a>.</p>

</body>
</html>
