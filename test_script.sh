/bin/ls
/bin/pwd
/usr/bin/env
/usr/bin/whoami
/bin/date
/usr/bin/touch file.txt
echo $?
/bin/rm file.txt
echo $?
/bin/echo hello world
echo $?
/bin/ls -l
echo $?
/bin/cat /etc/hostname
echo $?
/bin/date "+%Y-%m-%d"
echo $?
/bin/mkdir /tmp/test_dir
echo $?
/bin/rmdir /tmp/test_dir
echo $?
/bin/sleep 1
echo $?
/bin/ls filethatnotexist
expr $? + $?
/bin/echo "cat lol.c | cat > lol.c"
/usr/bin/touch file.txt
/bin/chmod 000 file.txt
/bin/echo "hey !" > file.txt
/bin/chmod 777 file.txt
/bin/rm file.txt
/bin/echo ""
/bin/echo "hello world" "foo bar" baz
/bin/echo "this is a    test" "   spaced" end
/bin/echo 'this is a test'
/bin/echo ''
/bin/echo '$USER'
/bin/echo $USER
/bin/echo "$USER"
/bin/echo 'some | weird > stuff'
echo "/bin/echo Hello from here" > test.sh
chmod +x test.sh
/bin/sh ./test.sh
/bin/sh ./minishell/test.sh
