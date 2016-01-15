#include<stdio.h>
#include<stdlib.h>
typedef struct binary_tree_node
{
	int data;
	int priority;
	int token;
	struct binary_tree_node *left_child;
	struct binary_tree_node *right_child;
	struct binary_tree_node *parent;
}heap;

typedef struct queue_node
{
	heap *address;
	struct queue_node *link;
}queue;

void enqueue(queue **front,queue **rear,heap *addin)
{
	queue *pnew;
	pnew=(queue *)malloc(sizeof(queue));
	if(!pnew)
	{
		printf("\nMemory Allocation Failed\n");
		return;
	}
	pnew->address=addin;
	pnew->link=NULL;
	if(*front==NULL)
		*front=pnew;
	else
		(*rear)->link=pnew;
	*rear=pnew;
}
void enqueue_front(queue **front,queue **rear,heap *addin)
{
		
	queue *pnew;
	pnew=(queue *)malloc(sizeof(queue));
	if(!pnew)
	{
		printf("\nMemory Allocation Failed\n");
		return;
	}
	pnew->address=addin;
	pnew->link=*front;
	if(*front==NULL)
		*rear=pnew;
	*front=pnew;
	
	
}

heap *dequeue(queue **front,queue **rear)
{
	heap *dataout;
	queue *pfront;
	if(*front==NULL)
	{
		printf("\nQueue is Empty\n");
		return NULL;
	}
	pfront=*front;
	*front=pfront->link;
	dataout=pfront->address;
	if(*front==NULL)
		*rear=NULL;
	free(pfront);
	return dataout;
}

heap *queue_front(queue *front)
{
	heap *dataout=front->address;
	return dataout;
}

heap *queue_rear(queue *rear)
{
	heap *dataout=rear->address;
	return dataout;
}


void queue_last(queue **front,queue **rear,heap **last)
{
	queue *prear=*rear;
	heap *dataout;
	if(*front==NULL)
	{
		printf("\n Queue is Empty\n");
		return;
	}
	while(*front!=prear)
	{
		dataout=dequeue(front,rear);
		enqueue(front,rear,dataout);
	}
	dataout=dequeue(front,rear);
	*last=(*rear)->address;
}
void swap(heap **temp,heap **temp_parent)
{
	int tdata,tpriority;
	tdata=(*temp)->data;
	(*temp)->data=(*temp_parent)->data;
	(*temp_parent)->data=tdata;
	tpriority=(*temp)->priority;
	(*temp)->priority=(*temp_parent)->priority;
	(*temp_parent)->priority=tpriority;
	heap *node=*temp;
	*temp=*temp_parent;
	*temp_parent=node;
}

void heap_up(heap **node)
{
	heap *temp=*node;
	while(temp->parent!=NULL){
		heap *temp_parent=temp->parent;
		if(temp->priority>temp_parent->priority)
			swap(&temp,&temp_parent);
		else 
			break;
	
	}

}

void Insert(heap **root,heap **last,int datain,int priority_in,queue **front,queue **rear,int tok)
{
	heap *node,*temp;
	node=(heap *)malloc(sizeof(heap));
	if(!node)
	{
		printf("\nMemory Allocation Failed\n");
		return;
	}
	node->data=datain;
	node->priority=priority_in;
	node->left_child=NULL;
	node->right_child=NULL;
	node->token=tok;	
	if(*root==NULL)
	{						//For Empty heap
		*root=node;
		(*root)->parent=NULL;
		*last=*root;
		*front==NULL;
		*rear==NULL;
	}
	else
	{								//For Previously Present heap
		temp=*root;
		while(temp!=NULL)
		{
			if(temp->left_child==NULL)
			{
				temp->left_child=node;
				node->parent=temp;
				enqueue(front,rear,node);
				*last=node;
				break;
			}
			else if(temp->right_child==NULL)
			{
				temp->right_child=node;
				node->parent=temp;
				enqueue(front,rear,node);
				*last=node;
				break;
			}
			else
			{
				temp=queue_front(*front);
				if(temp->left_child!=NULL)
					temp=dequeue(front,rear);
			}
		}	
		//Traverse Breadth Wise to last node
		heap_up(&node);						//Converts Invalid heap into a Valid heap
	}
	return;
}

void exchange(heap **root,heap **last)
{
	int temp;
	temp=(*last)->data;
	(*last)->data=(*root)->data;
	(*root)->data=temp;
	temp=(*last)->priority;
	(*last)->priority=(*root)->priority;
	(*root)->priority=temp;
	temp=(*last)->token;
	(*last)->token=(*root)->token;
	(*root)->token=temp;
}

void heap_down(heap **root)
{
	if((*root)->left_child!=NULL&&(*root)->right_child==NULL&&(((*root)->left_child)->priority >= ((*root)->priority)))
	{	
		exchange(root,&((*root)->left_child));
		heap_down(&((*root)->left_child));
	}
	else if((*root)->left_child!=NULL&&(*root)->right_child!=NULL)
	{	
		heap *large_subtree;
		//printf("%d ",(*root)->data);	
		if(((*root)->left_child)->priority>((*root)->right_child)->priority)
		{
			exchange(root,&((*root)->left_child));
			heap_down(&((*root)->left_child));
		}
		
		else if(( ((*root)->left_child)->priority==(((*root)->right_child)->priority)&&( ((*root)->token>((*root)->right_child)->token)||((*root)->token>((*root)->left_child)->token))) )
		{
			if(((*root)->left_child)->token<((*root)->right_child)->token)
			{
				exchange(root,&((*root)->left_child));
				heap_down(&((*root)->left_child));
			}
			/*exchange(root,&((*root)->left_child));
			exchange(&((*root)->left_child),&((*root)->right_child));
			heap_down(&((*root)->right_child));*/
			else
			{
				exchange(root,&((*root)->right_child));
				heap_down(&((*root)->right_child));
			}	
		}		
		else
		{		
			exchange(root,&((*root)->right_child));
			heap_down(&((*root)->right_child));
		}
	}
	else
		return;
}

heap *delete_root(heap **root,heap **last,queue **front,queue **rear)
{	
	heap *data_out;	
	if(*root==NULL)
	{
		
		data_out=NULL;
		return ;
	}
	int temp;
	if(*last==*root)
	{
		data_out=*root; 
		//data_out=dequeue(front,rear);
		*last=NULL;
		*root=NULL;
		return data_out;     
	}
	if((*last)->parent!=(*front)->address)
		enqueue_front(front,rear,(*last)->parent);
	exchange(root,last);
	/*temp=(*last)->data;
	(*last)->data=(*root)->data;
	(*root)->data=temp;
	temp=(*last)->priority;
	(*last)->priority=(*root)->priority;
	(*root)->priority=temp;*/
	data_out=*last;
	
	if(((*last)->parent)->right_child==*last)
	{		
		((*last)->parent)->right_child=NULL;	
		*last=((*last)->parent)->left_child;	
	}
	else
	{
		((*last)->parent)->left_child=NULL;	
		*last=(*last)->parent;	
	}
	heap_down(root);
	queue_last(front,rear,last);
	
	return data_out;
}
void display()
{
	//usleep(500000);
	system("clear");
//	clrscr();
	printf("\n\n **************************************** DS MINI PROJECT **************************************** ");
	printf("\n  SAMEER S. KHURD\t\t\t\t\t\t\t\tASHUTOSH N. SITRE");
	printf("\n  2014BCS036\t\t\t\t\t\t\t\t\t2014BCS058");
	printf("\n ------------------------- IMPLEMENTATION OF PRIORITY QUEUE USING HEAP ---------------------------\n	");
}

void display_tree(heap *root,int level,int t)
{
	int i;
	if(root!=NULL)
	{
		for(i=1;i<=level;i++)
			printf("\t");
		if(t==2)
			printf("| %d (%d) %d\n",root->data,root->priority,root->token);
		else
			printf("| %d \n",root->data);		
		display_tree(root->left_child,level+1,t);
		display_tree(root->right_child,level+1,t);		
	}
}		
void file_input(heap **root,heap **last,queue **front,queue **rear,int token){
	int nodes,data,pr,i;
	FILE *fp;
	fp=fopen("input.txt","r");
	if(fp==NULL){
		printf("\n  File cannot be opened\n");
		return;
	}
	else
		printf("\n  File has been successfully opened\n");	
	fscanf(fp,"%d",&nodes);
	for(i=0;i<nodes;i++){
		fscanf(fp,"%d%d",&data,&pr);
		Insert(root,last,data,pr,front,rear,++token);
	}
	fclose(fp);
}

void no_of_nodes(heap *root,int *non){
	if(root!=NULL){
		*non=*non+1;
		no_of_nodes(root->left_child,non);
		no_of_nodes(root->right_child,non);
	}
}

//FILE *fr;
void save(heap *root,FILE **fr){
	if(root!=NULL){
		fprintf(*fr,"%d %d\n",root->data,root->priority);
		save(root->left_child,fr);
		save(root->right_child,fr);
	}
}


int main()
{
	char z;
	int datain,priority=1,p,flag=0,fs=0,a,t,o;
	int non,seat;
	FILE *fr;
	while(1)
	{
		display();
		step:
		printf("\n  Main Menu\n\n\t[1] Create Your Own Queue\n\t[2] Load Queue From File\n\t[3] Application\n\t[4] Exit\n\n  Choice: "); 
		a=0;		
		scanf("%d",&a);
		//z=getchar();
		int token=0;
		heap *root=NULL,*last=NULL,*data_out_pointer,*temp;
		queue *front=NULL,*rear=NULL;
		if(a==4)
			return 1;
		switch(a)
		{
			case 1:
				while(1)
				{
					display();
					printf("\n  Create a Queue\n\n\t[1] Create Implicit Priority Queue\n\t[2] Create Explicit Priority Queue\n\t[3] Back to Main Menu\n\t[4] Exit\n\n  Choice: "); 
					scanf("%d",&t);	
					//z=getchar();
					if(t==4)
						return 1;
					if(t==3)
						break;
					while(t==1||t==2)
					{		
						display();
						if(t==1)	
							printf("\n  Implicit Priority Queue Menu\n\n\t[1] Enqueue an Element\n\t[2] Dequeue an Element \n\t[3] Indented Display\n\t[4] Save Queue in File\n\t[5] Back to Main Menu\n\t[6] Exit\n\n  Choice: ");
						else if(t==2)
							printf("\n  Explicit Priority Queue Menu\n\n\t[1] Enqueue an Element\n\t[2] Dequeue an Element \n\t[3] Indented Display\n\t[4] Save Queue in File\n\t[5] Back to Main Menu\n\t[6] Exit\n\n  Choice: ");
						scanf("%d",&o);
						//z=getchar();
						if(o==5)
						{
							flag=1;
							break;
						}	
						if(o==6)
							return 1;
						switch(o)
						 {
							case 1:								
								printf("\n  Enter Data: ");
								scanf("%d",&datain);
								if(t==2)	
								{	
									printf("  Enter Priority: ");
									scanf("%d",&priority);
								}
								else
									priority=datain;					
								Insert(&root,&last,datain,priority,&front,&rear,++token);
					 			break;
							case 2:	
								if(root!=NULL)
								{
									data_out_pointer=delete_root(&root,&last,&front,&rear);
									if(t==2)
										printf("\n  Dequeued Element: %d [priority: %d]",data_out_pointer->data,data_out_pointer->priority); 
									else
										printf("\n  Dequeued Element: %d",data_out_pointer->data); 
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	
									temp=data_out_pointer;
									if(temp!=root)										
										free(temp);	
								}					
								else{
									printf("\n  Sorry, cannot dequeue element because Queue is empty");
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	}					
								break;
							case 3:
								if(root!=NULL)
								{
									printf("\n  Indented Display:\n ");
									display_tree(root,0,t);					
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);						
									printf("\n");
								}
								else{
									printf("\n  Sorry, cannot display Queue because it is empty");
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	
									}
								break;
							case 4:	
								//FILE *fr;
								fr=fopen("output.txt","w");
								non=0;
								no_of_nodes(root,&non);
								fprintf(fr,"%d\n",non);
								save(root,&fr);
								fclose(fr);	
							
				  			default:
								break;
				   		}
			
					}
					if(flag==1)
						break;
				}
				break;	
		case 2:
			file_input(&root,&last,&front,&rear,token);
			
			while(1)
			{
				display();
				printf("\n  Menu\n\n\t[1] Enqueue an Element\n\t[2] Dequeue an Element\n\t[3] Indented Display\n\t[4] Save Queue in File\n\t[5] Back to Main Menu\n\t[6] Exit\n\n  Choice: ");
				scanf("%d",&o);
				if(o==6)
					return 1;
				if(o==5)
					break;
				switch(o)
				{
							case 1:								
								printf("\n  Enter Data: ");
								scanf("%d",&datain);
								printf("  Enter Priority: ");
								scanf("%d",&priority);
								Insert(&root,&last,datain,priority,&front,&rear,++token);
					 			break;
							case 2:	
								if(root!=NULL)
								{
									data_out_pointer=delete_root(&root,&last,&front,&rear);
									printf("\n  Dequeued Element: %d [priority: %d]",data_out_pointer->data,data_out_pointer->priority); 
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	
									temp=data_out_pointer;
									if(temp!=root)										
										free(temp);	
								}					
								else{
									printf("\n  Sorry, cannot dequeue element because Queue is empty");
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	}					
								break;
							case 3:
								if(root!=NULL)
								{
									printf("\n  Indented Display:\n ");
									display_tree(root,0,2);					
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);						
									printf("\n");
								}
								else{
									printf("\n  Sorry, cannot display Queue because it is empty");
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	
									}
							case 4:	
								//FILE *fr1;
								fr=fopen("output.txt","w");
								non=0;
								no_of_nodes(root,&non);
								fprintf(fr,"%d\n",non);
								save(root,&fr);
								fclose(fr);	
							
				  			default:
							break;
				}
			}
			break;
		case 3:
			//file_input(&root,&last,&front,&rear);
			
			while(1)
			{
				display();
				printf("\n  Application Menu\n\n\t[1] Apply for Seat\n\t[2] Confirmed Seats\n\t[3] Indented Display\n\t[4] Back to Main Menu\n\t[5] Exit\n\n  Choice: ");
				scanf("%d",&o);
				if(o==5)
					return 1;
				if(o==4)
					break;
				switch(o)
				{
							case 1:								
								printf("\n  Enter ID: ");
								scanf("%d",&datain);
								printf("  Choose from following Category:\n\t[1] Dependents\n\t[2] Enlisted Personnel\n\t[3] Off Duty Officers\n\t[4] On Duty Officers\n\t[5] Official Trip\n\t[6] Rescued Hostages\n\t[7] Medical Emergencies\n\n  Choice:");  					
								//printf("  Enter Priority: ");
								scanf("%d",&priority);
								Insert(&root,&last,datain,priority,&front,&rear,++token);
					 			break;
							case 2:	
								printf("\n Enter Available Seats: ");
								scanf("%d",&seat); 
								while(root!=NULL&&seat--)
								{
									data_out_pointer=delete_root(&root,&last,&front,&rear);
									printf("\n  Confirmed ID: %d ",data_out_pointer->data); 
									//printf("\n  Enter any Integer to continue: ");
									//scanf("%d",&p);	
									temp=data_out_pointer;
									if(temp!=root)										
										free(temp);	
								}
								printf("\n\n  Enter any Integer to continue: ");
									scanf("%d",&p);						
								/*else{
									printf("\n  Sorry, cannot dequeue element because Queue is empty");
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	}	*/				
								break;
							case 3:
								if(root!=NULL)
								{
									printf("\n  Indented Display:\n ");
									display_tree(root,0,2);					
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);						
									printf("\n");
								}
								else{
									printf("\n  Sorry, cannot display Queue because it is empty");
									printf("\n  Enter any Integer to continue: ");
									scanf("%d",&p);	
									}
							
							
				  			default:
							break;
				}
			}
			break;	
		}
	//	fflush(stdin);
	}
	return 1;
}
