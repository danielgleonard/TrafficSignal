class Message {
  private:
    String message;
    String destination;
    bool valid;
    bool completed;
    String parseCommand(String command) {
      message = command;
    }
  public:
    Message(String inputStream) {
      message = parseCommand(inputStream);
    }
    Message(char[] inputStream) {
      Message(String(inputStream));
    }
    
    bool isValid() {
      
    }
    int getargc() {
      
    }
    *char[] getargv() {
      
    }
}
