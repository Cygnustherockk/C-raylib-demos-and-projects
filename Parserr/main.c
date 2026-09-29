  #include "parser.h"

  int main(void)
  {
    ItemCollection col = GetFileData("example.txt");
    for(int i = 0; i < col.count; i++)
    {
      printf("%s, %s \n", col.items[i].item_name, col.items[i].item_value);
    }
    return 0;
  }