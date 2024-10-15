#include "Transaction.h"

Transaction::Transaction() {
    id = "secret_dxdMsM8Ybc26AwICFk8hHrruOtJwz0zJbvi6QDfh2NH";
    database = "94722e487e7848c3b562a7bb19493596";//32 characters long. numbers and letters.
    text = "";
    requestBody = "";
    block = "";
    timestamp = "";
}

void Transaction::timeStamp() {
    //calculate timestamp
    const auto p1 = system_clock::now();
    const auto intTimestamp = duration_cast<milliseconds>(p1.time_since_epoch()).count();
    timestamp = to_string(intTimestamp);
}

void Transaction::getReciept(string ty, string pa, string pr) {
    //setup
    timeStamp();

    //textStructure
    text = "New Order! Details As Follows:\n";
    text = text + "TimeStamp: " + timestamp + "\n";
    text = text + "Type: " + ty + "\n";
    text = text + "Pair: " + pa + "\n";
    text = text + "Price: " + pr + "\n";

    //structure requestst and send request
    body();
    requests();
}

void Transaction::body() {
    json children;
    json childArr;
    json paragraph;
    json richText;
    json type;
    json text;
    json content;

    content = { {"content", this->text} };
    text = { {"type", "text"}, {"text", content} };
    type.push_back(text);
    richText = { {"rich_text", type} };
    paragraph = {
        {"object", "block" },
        {"type", "paragraph"},
        {"paragraph", richText}
    };
    childArr.push_back(paragraph);
    children = { {"children", childArr} };
    block = children.dump();
}

void Transaction::requests() {
    string url = "https://api.notion.com/v1/blocks/94722e48-7e78-48c3-b562-a7bb19493596/children";
    string bearerToken = "Bearer " + id;
    string contentType = "application/json";

    Response r;

    Header header;
    header.insert({ "Authorization", bearerToken });
    header.insert({ "Content-Type", contentType });
    header.insert({ "Notion-Version", "2022-02-22" });//*

    r = Patch(Url{ url }, Body{ block }, Header{ header });
    //LLOD
}
