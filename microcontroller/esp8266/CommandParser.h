#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
  public:
    Message(String str);
//    Message(*char str);

    int isValid();
    int getArgc();
};

#endif
