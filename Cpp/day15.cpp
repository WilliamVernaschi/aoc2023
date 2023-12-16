#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp>


using namespace std;
using namespace __gnu_pbds;


struct Item{
  string label;
  int lens;

  Item(string label, int lens){
    this->label = label;
    this->lens = lens;
  }
};

// Insertion O(1)
// Removal O(1)
// Get all elements O(#elements)
struct Box{
  private:
    list<Item> elements;
    unordered_map<string, std::list<Item>::iterator> element_pointer;

  public:
    void insert(Item item){

      if(element_pointer.count(item.label)){
        (*element_pointer[item.label]).lens = item.lens;
      }
      else{
        elements.push_back(item);
        element_pointer[item.label] = prev(elements.end());
      }
    }

    void pop(string label){
      if(!element_pointer.count(label)) return;

      elements.erase(element_pointer[label]);
      element_pointer.erase(label);
    }

    vector<Item> items(){
      vector<Item> i;
      for(Item item : elements)
        i.push_back(item);
      return i;
    }
};

// Insertion O(#elements)
// Removal O(#elemnts)
// Gets all elements O(#elements)
struct AsymptoticallySlowerButActuallyFasterBox{
  private:
    vector<Item> elements;

  public:
    void insert(Item item){

      for(Item &i : elements){
        if(i.label == item.label){
          i.lens = item.lens;
          return;
        }
      }
      elements.push_back(item);
    }

    void pop(string label){
      for(int i = 0; i < elements.size(); i++){
        if(elements[i].label == label){
          elements.erase(elements.begin() + i);
          break;
        }
      }
    }

    vector<Item> items(){
      return elements;
    }
};

int HASH(string text){
  int ans = 0;
  for(int i = 0; i < text.size(); i++){
      ans += text[i];
      ans *= 17;
      ans %= 256;
  }
  return ans;
};

int main(){
  string in; cin >> in;
  in += ',';
  string text;

  int ans = 0;
  for(int i = 0; i < in.size(); i++){
    if(in[i] == ',') ans += HASH(text), text = "";
    else text += in[i];
  }

  cout << "Part 1: " << ans << '\n';

  vector<Box> boxes(256);
  for(int i = 0; i < in.size(); i++){
    if(in[i] == ','){
      string label;
      if(text[text.size()-2] == '='){
        label = text.substr(0, text.size()-2);

        boxes[HASH(label)].insert(Item(label, text.back()-'0'));
      }
      else{
        label = text.substr(0, text.size()-1);
        boxes[HASH(label)].pop(label);
      }
      text = "";
    }
    else text += in[i];
  }

  long long part2 = 0;
  for(int i = 0; i < 256; i++){

    int j = 0;
    for(Item item : boxes[i].items()){
      part2 += (long long)(i+1) * item.lens * (j+1);
      j++;
    }
  }

  cout << "Part 2: " << part2 << '\n';
}
