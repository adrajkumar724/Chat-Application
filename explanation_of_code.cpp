#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

//user class stores the Information about 
//username
//password
// list of friends he/she have
// list of Messages sent to whom at which time
//list of received from whom at Which time

class User{
    public:
    string uname;
    string pass;
    list<string> frnds;
    list<pair<string, pair<string, string>>> sent, received;
    User(string username){
      uname= username;
      cout<<"Enter password: ";
      cin>> pass;
    }
};
//for avl tree


class Node{
public:
    User *user;
    Node *right, *left;
    int height;
};
Node *root= NULL;
int height(Node* node){
    if(node==NULL){
        return 0;
    }
    else{
        return node->height;
    }
}
int getBF(Node* node){
    return(height(node->left)-height(node->right));
}
Node* Newnode(string uname){
    Node *NNode=(Node*) malloc (sizeof(Node));
    NNode->user= new User(uname);
    NNode->left=NNode->right=NULL;
    NNode->height=1;
    return NNode;
}
Node* RRot(Node *z){
    Node *y=z->left;
    Node *T2=y->right;

    y->right=z;
    z->left=T2;

    y->height=1+max(height(y->left),height(y->right));
    z->height=1+max(height(z->left),height(z->right));

    return y;
}
Node* LRot(Node *x){
    Node *y=x->right;
    Node *T1=y->left;

    y->left=x;
    x->right=T1;

    y->height=1+max(height(y->left),height(y->right));
    x->height=1+max(height(x->left),height(x->right));

    return y;
}
Node* insert(Node* node, string uname){
    if(node==NULL) return Newnode(uname);
    else if(uname > node->user->uname){
        node->right=insert(node->right,uname);
    }
    else if(uname < node->user->uname){
        node->left=insert(node->left,uname);
    }
    else{
        return node;
    }

    node->height=1+max(height(node->left),height(node->right));
    int Bf = getBF(node);
    if(Bf>1){
        if(uname < node->left->user->uname){
                return RRot(node);
        }
        else{
            node->left=LRot(node->left);
            return RRot(node);
        }
    }
    if(Bf<-1){
        if(uname > node->right->user->uname){
            return LRot(node);
        }
        else{
            node->right=RRot(node->right);
            return LRot(node);
        }
    }
    return node;
}
void inorder(Node *node){
    if(node!= NULL){
        inorder(node->left);
        cout<< node->user->uname <<" ";
        inorder(node->right);
    }
}

Node* searchUser(Node *root, string uname){
    
  if(root== NULL) return NULL;
  
  if(uname < root->user->uname) return searchUser(root->left, uname);
  
  else if(uname > root->user->uname) return searchUser(root->right, uname);
  
  else return root;
}

void addFriend(Node *curuser){
    
  //cur user is "Current user" Who wants to "add" a friend in cha APPLICATION
  
  string fname;
  cout<<"Enter the friend name: "; cin>>fname;
  
  //fname is the friend who we want to add as a friend
  //checking for its Availabiltiy
  
  
  Node *fnode= searchUser(root, fname);
  
  //If not available then Return "No such User available"
  
  if(fnode== NULL) cout<<"No such user available"<<endl;
  
  //if such user is Available in Chat APPLICATION
  // we are attempting to  make him as his Friend
  
  
  else{
      
     // we are searching in the friends list that "User" is available or not
     //If the User is not Available in the friends list .. list<string> frnds
     //Add them in your friend list
     
     
    if(find(curuser->user->frnds.begin(), curuser->user->frnds.end(), fname)== curuser->user->frnds.end()){
        
        // we are pushing back in the Friend's list
        
        
      curuser->user->frnds.push_back(fname);
      cout<<"Friend added successfully\n";
    }
    //else we are telling friend already exists
    
    else{
      cout<<"This friend already exists\n";
    }
  }

    
}

void sendMsg(Node *curuser){
    
	string msg;
	string fname;
	
	cout<<"Enter the Reciever name: "; cin>>fname;
	
	//Searching for the Entered user is present or Not
	
    Node *fnode= searchUser(root, fname);
    
    //if not present 
    
    if(fnode== NULL) cout<<"No such user available"<<endl;
    
    //Here the Username exists
    //Again We are checking whether the "receiver" is in Friends list of "Sender"
    
    
	else if(find(curuser->user->frnds.begin(), curuser->user->frnds.end(), fname)== curuser->user->frnds.end()){
		cout<<"Requested User not in Friend List";
	}
	
	//Only if both Above conditon Satisfy
	//the Sender is Allowed to Send the Message
	else{
		cout<<"Enter the Message: ";
		
        cin.ignore();
        
        getline(cin, msg);
        
        pair<string, pair<string, string>> temp;
        
        //time_t is an inbuild class
        //time(0) is used to find the current calendar time
        
        time_t now= time(0);
        
        //ctime returns the string representing the local time based on arguement timer
        
        
        string dt= ctime(&now);
        
        //storing the time in the form of string in the pair's first element
        
        
        temp.first= dt;
    
        //storing the username who "received" the message
       
        temp.second.first= fnode->user->uname;
        
        //storing the message
        
        temp.second.second= msg;
        
        // list<pair<string,pair<string,string>> sent,received
        
        // we are pushing the pair into "sent" list
        
		curuser->user->sent.push_back(temp);
		
		//storing the username who "sent" the Messages
		
		// just change the receiver name to sent name
		
		// this stores the details of time ,"sender username", message
		
		
        temp.second.first= curuser->user->uname;
        
        
        //store this temp into List containing received Messages
        
        fnode->user->received.push_back(temp);
		
		cout<<"\nMessage Sent Successfully\n";
	}

}

//This functions Display Messages present in the inbox

// we are curuser who message's to be Displayed

void inboxdisp(Node *curuser){
	cout<<"\nRecieved Messages: \n\n";
	
	
	list<pair<string, pair<string, string>>> :: iterator ptr; //It is an Iterator which is used to point at the 
	// memory address of STL containers
	
	// list<pair<string,pair<string,string>> sent ,received containes the Sent and received Messages of that curuser
	
	// To display Messages present in the inbox
	
	// we are traversing through the received Messages's list and printing the Messages
	
	for(ptr=curuser->user->received.begin();ptr!=curuser->user->received.end();ptr++){
	            // prints the time           // prints the SenderName    //prints the Messages
		cout<<"["<<(*ptr).first<<"]"<< " "<< (*ptr).second.first << " : " <<(*ptr).second.second<<"\n";
	}
	cout<<"\n\n";
}

// This Function present in the Outbox ie the Messages we sent
// Curuser is Current User whose Outbix we trying to print


void outboxdisp(Node *curuser){
	cout<<"\nSent Messages: \n\n";
	list<pair<string, pair<string, string>>> :: iterator ptr;
	// To display Messages present in the Outbox
	
	// we are traversing through the Sent Messages's list and printing the Messages
	for(ptr=curuser->user->sent.begin();ptr!=curuser->user->sent.end();ptr++){
		cout<<"["<<(*ptr).first<<"]"<< " "<< (*ptr).second.first<<" : " << (*ptr).second.second<<"\n";
	}
	cout<<"\n\n";
}

//This Function display the Friend of a user


void frnddisp(Node *curuser){
	cout<<"\nFriends: ";
	list<string> :: iterator ptr;
	
	// we are taking Curuser's username and traversing through its list of Friends ..list<string> frnds
	
	// we are printing them 
	
	for(ptr=curuser->user->frnds.begin();ptr!=curuser->user->frnds.end();ptr++){
		cout<<*ptr<<"\n";
	}
	
	cout<<"\n\n";
}

//this function Activate the User who have Created an account (Signed up) and did Login


void activeUser(Node *curuser){
  int choice;
  while(1){
		cout<<"\n1. Add friend\n2. Send message\n3. Inbox\n4. Outbox\n5.Friend list\n6. Exit\n\n";
    cout<<"Enter your choice: ";
    
    cin>>choice;
    if(choice== 1){
      //add friend
      addFriend(curuser);
    }else if(choice== 2){
      //send message
      sendMsg(curuser);
    }else if(choice== 3){
      //inbox
      inboxdisp(curuser);
    }else if(choice== 4){
      //outbox
		outboxdisp(curuser);
    }else if(choice==5){
			//Friend List
		frnddisp(curuser);
	}
	else if(choice==6){
	    exit(0);
	}
	
  }
}
int main(){
    string uname, pass;
    int choice;
    while(1){
		cout<<"-- CHAT APPLICATION --";
    	cout<<"\n1. Login\n2. Signup\n\n";
        cout<<"Enter your choice: ";
        cin>>choice;
        
         if(choice== 1){
              cout<<"Enter your username: "; cin>>uname;
              cout<<"Enter the password: "; cin>>pass;
              Node *curuser= searchUser(root, uname);
              if(curuser!= NULL){
                  
                    if(curuser->user->pass== pass) activeUser(curuser);
            
                    else cout<<"incorrect pass"<<endl;
              
                  
                 }
        }
        else if(choice== 2) 
        {
                cout<<"Enter your username: "; cin>>uname;
                if (searchUser(root, uname)== NULL){
                        root= insert(root, uname) ;
                        cout<<"signed up"<<endl;
                }
                 
		        else cout<<"already exits"<<endl;
        }   
        else{
                exit(0);
        }
    }
    return 0;
}
