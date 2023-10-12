#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <memory>
#include <mutex>
#include<thread>
using namespace std;

mutex mutexOrder;


string encryptFile(string& input,int key)
{

    string encrypt = input;
    for(char& c : encrypt)
    {
        c = static_cast<char>(c+key);
    }

    return encrypt;


}

string dencryptFile(string& output,int key)
{

    string encrypt = output;
    for(char& c : encrypt)
    {
        c = static_cast<char>(c-key);
    }

    return encrypt;


}

class BankSystem 
{

    private:
    double balance;
    bool blocked;
    public :
    
    BankSystem()
    {
        this -> balance;
        this -> blocked = false;
    }

    

    void accBlocked(){this->blocked = true;}
    void accUnblocked(){this->blocked = false;}
    

    double inputAmount()
    {
        double amount;      
        cin >> amount;
        return amount;
    }  
      
    void addAmount()
    {
        cout << "Enter the amount to deposit" << endl;
        double amontAdd = inputAmount();
        this->balance +=amontAdd;
        cout << "Added " << amontAdd << " successfully." << endl;
        cout << "Current balance: " << this->balance << endl;

    }

    void WithdrawAmount()
    {   
        cout << "Enter the amount to withdraw" << endl;
        double amontWithdraw = inputAmount();
         if(amontWithdraw <= this->balance)
         {
            this->balance -= amontWithdraw;
            cout << "Withdraw :" << amontWithdraw <<" successfully." << endl;
            cout << "Current balance :" <<this->balance << endl;

         }   
         else
         {
            cout << "Withdraw unsuccessful \n" <<endl;
         }
    }
    

    void CheckBalance()
    {

        cout << "Current balance :" << this->balance << endl;
    }

    double getBalance()
    {
        return this->balance;
    }

    ~BankSystem()
    {   
        cout << "You account has been deleted" << endl;
        
    }
    
   
};


class Person : public BankSystem

{
    private :
    string name;
    string lastname;
    string address;
    int day;
    int month;
    int year;
    int ID;

  
    public:

    Person(){}

    void setUser()
    {
        cout << "Enter your ID number :" << endl;
        cin >> this->ID;
        cin.ignore();
        cout << "Enter your name :" << endl;
        getline(cin,this->name);
        if(this->name.empty())
        {
            cout << "Invalid name" << endl;
        }
       

       cout << "Enter your last name :" << endl;
        getline(cin,this->lastname);
        if(this->lastname.empty())
        {
             cout << "Invalid lastname" << endl;
        }

        cout << "Enter your address :" << endl;
        getline(cin,this->address);
        if(this->address.empty())
        {
             cout << "Invalid address :" << endl;
        }

        cout << "Enter you date of birth (MM/DD//YY) :" << endl;
        cin >> this->day;
        cin.ignore();
        cin >> this->month;
        cin.ignore();
        cin >> this->year;
        cin.ignore();

    }                

    string getAddress() {return this->address;}    
    string getLastName() {return this->lastname;}
    string getName() {return this->name;}
    int getDay() {return this->day;}
    int getMonth() {return this->month;}
    int getYear() {return this->year;}
    int getID() { return this->ID ;}
    
    void printInfoUser() 
    {
        
        cout << "Name of the User : " << getName() << endl;
        cout << "Last Name of the User : " << getLastName() << endl;
        cout << "Address of the User : " << getAddress() << endl;
        cout << "ID number of the User :" << getID()<<endl;
        if(getYear() > 0 || getYear() < 2099 && getMonth() > 0 || getMonth () < 12 && getDay() > 1 || getDay() > 0 || getDay() < 31)
        {
        cout << "Date of Birth :" << getDay() << "/" << getMonth() << "/" << getYear() << endl;
        }
        else
        {
             cout << " Invalid date of birth " << endl;
  
        }

        cout << "Current Balance :" << getBalance() << endl;
        


    }
    
};



int main ()
{   

    const int users = 2;
    //shared_ptr <BankSystem> account[users];
    shared_ptr <Person> useraccount [users];
   

    for(int i = 0 ; i < users; i++ )
    {
        useraccount[i] = make_shared<Person>();
        useraccount[i]->setUser();
        int id ;
        cout << "Enter the ID number for security checkout" << endl;
        cin >> id ;
        if(id == useraccount[i]->getID())
        {
            
            useraccount[i]->accUnblocked();
            
            cout << "You ID is matched" << endl;
        
        //account.inputAmount();
        useraccount[i]->addAmount();
        useraccount[i]->WithdrawAmount();
        cout << "User final Balance :" << useraccount[i]->getBalance() <<endl;

            if(useraccount[i]->getBalance() == 0)
            {
                
                int option;

                cout <<" 1 : Deposit certain amount : " << endl;
                cout <<" 2 : Delete account :" << endl;

                cin >> option;

                switch (option)
                {
                case 1:
                     useraccount[i]->addAmount();
                    break;
                case 2:
                    useraccount[i].reset();
                    if (i + 1 < users) {
                    cout << "Account deleted. Moving to the next slot." << endl;
                    i++;
                     } 
                     else {
                     cout << "No more slots for new accounts." << endl;
                        }
                    continue;
                }

            } 


        }
           
   
    
    else
    {
         cout << "You ID is not matched" << endl;

    }


        int choice;
        
        cout << "1. Write to file\n";
        cout << "2. Read from file\n";
        
        cin >> choice;
        if(choice == 1)
            {
                
                ofstream OpenFIle("UsersDatabase.txt",ios::app);
                if (OpenFIle.is_open()) { 
                lock_guard<mutex> lock(mutexOrder); // order for wrting in the file
                string userName = useraccount[i]->getName();
                OpenFIle << "Name of the User : " << encryptFile(userName,5) << endl;
                string userLastName = useraccount[i]->getLastName();
                OpenFIle << "Last Name of the User :" << encryptFile(userLastName,5) << endl;
                string userAddress = useraccount[i]->getAddress();
                OpenFIle << "Address of the User :" << encryptFile(userAddress,5)<<endl;
                string id = to_string(useraccount[i]->getID());
                OpenFIle << "ID number of the User :" << encryptFile(id,5) <<endl;
                string day = to_string(useraccount[i]->getDay());
                string month = to_string(useraccount[i]->getMonth());
                string year = to_string(useraccount[i]->getYear());
                if(useraccount[i]->getYear() > 0 || useraccount[i]->getYear() < 2099 && useraccount[i]->getMonth() > 0 || useraccount[i]->getMonth () < 12 && useraccount[i]->getDay() > 1 || useraccount[i]->getDay() > 0 || useraccount[i]->getDay() < 31)
                {
                    OpenFIle << "Date of Birth :" << encryptFile(day,5) << "/" << encryptFile(month,5) << "/" << encryptFile(year,5) << endl;
                }
                else
                {
                    cerr << " Invalid date of birth " << endl;
                }

                string balance = to_string(useraccount[i] ->getBalance());
                OpenFIle << "Current Balance :" << encryptFile(balance,5)<<endl;
                OpenFIle.close();
            }
                else 
            {
                cerr << "Unable to open the file for writing." << endl;
            }

            } if (choice == 2 )

            {
               
                
                ifstream ReadFile("UsersDatabase.txt");
                 if(!ReadFile.is_open())
                 {
                cerr << "Cound not open the file" << endl;
                return 1;
                 }
                
                string line;
                //mutexOrder.lock(); lock_guard auto lock() and unlock()
                while(getline(ReadFile,line))
                {
                    lock_guard<mutex> lock(mutexOrder); // order for reading from the file
                 
                    cout << line << endl;

                    
                    
                }

                ///mutexOrder.unlock();

                ReadFile.close();
            }

   
       

        cout << "User count: " << useraccount[i].use_count() << endl;

    }





    
    return 0;
    
    

}