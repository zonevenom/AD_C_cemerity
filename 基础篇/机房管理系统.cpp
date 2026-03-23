#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
#include<ctime>

using namespace std;

/*常量定义*/
const int ROOM_COUNT = 6; //教室数量
const int HANG_ROOM = 10; //每间教室行数
const int LIE_ROOM = 5; //每间教室列数
const int MAX_COMPUTERS = ROOM_COUNT * HANG_ROOM * LIE_ROOM; //电脑总数量
const int MAX_HISTORY = 5; //每台电脑最大历史访问记录数
const string DATE_FILE = "E://test//lab_data.txt"; 

/*枚举定义*/
enum ComputerStatus{
    AVAILABLE = 0, //可用
    OCCUPIED = 1, //使用中
    MAINTENANCE = 2 //维修
};
enum WeekDay{
    MON = 1,TUE,WED,THU,FRI
};

/*结构体定义*/
struct TimeRange //使用时间段
{
    string startTime;
    string endTime;
};
struct UseRecord //使用记录结构
{
    string studentName;
    string studenId;
    TimeRange time;
    bool isValid; //是否被使用
};
struct Schedule //课程安排
{
    int day; //周一到周五
    int period; //当天的第几节课
    string courseName;
    string teacherName;
    bool isSchedule; //是否已经排课
};
struct Computer //计算机结构
{
    int id;
    int roomid;
    int hang;
    int lie;

    string config; //电脑配置
    ComputerStatus statu;

    UseRecord history[MAX_HISTORY]; //历史记录数组
    int historyCount; //当前记录条数

    //当前使用者信息
    string currentUser;
    string currentId;
    string loginTime;
};
struct Room //教室结构
{
    int id;
    Schedule schedules[40]; //一天八节课一周5天
    int scheduleCount; //此教室课程数量
};

/*全局变量*/
Computer computers[MAX_COMPUTERS]; //所有电脑的状态
Room rooms[ROOM_COUNT]; //所有教室的状态

/*辅助函数*/
string getStatusStr(ComputerStatus s){ //获取状态字符串
    if (s == AVAILABLE) return "可用";
    if (s == OCCUPIED) return "使用中";
    if (s == MAINTENANCE) return "维修中";
    return "未知";
}
string getDayStr(int d) {  // 获取星期字符串
    switch(d) {
        case MON: return "周一";
        case TUE: return "周二";
        case WED: return "周三";
        case THU: return "周四";
        case FRI: return "周五";
        default: return "未知";
    }
}
string getCurrentTime(){  //获取系统时间
    time_t now =time(0);
    tm* localTime = localtime(&now);
    char buff[80];
    strftime(buff,sizeof(buff),"%H:%M",localTime);  //(存放点，内存大小，输出格式，当前时间指针)
    return string(buff);
}
Computer* findComputer(int id){ //查找当前计算机指针
    for(int i=0;i<MAX_COMPUTERS;i++){
        if(computers[i].id == id){
            return &computers[i];
        }
    }
    return nullptr;
}
/*核心功能*/
//1.初始化教室排课和电脑配置数据
void initData(){
    int idx =0 ;
    for(int r=1;r<ROOM_COUNT;++r){
        rooms[r-1].id = r;
        rooms[r-1].scheduleCount = 0;
    
    //初始化课表 教室一周40节课都没排
    for(int k = 0;k<40;k++) rooms->schedules[k].isSchedule =false;
    //初始化电脑数据
    for(int hang = 1;hang <=HANG_ROOM;hang++){
        for(int lie =1;lie<=LIE_ROOM;lie++){
            computers[idx].id = idx + 1;
            computers[idx].roomid = r;
            computers[idx].hang = hang;
            computers[idx].lie = lie;
            computers[idx].config = "i5-12400F, 16GB RAM, Win11";
            computers[idx].statu = AVAILABLE;
            computers[idx].historyCount = 0;
            computers[idx].currentUser="";
            //初始化历史数据
            for(int h =0;h<MAX_HISTORY;h++){
                computers[idx].history[h].isValid = false;
            }
            idx++;
        }
      }
    }
    cout<<">>>系统初始化完毕：共加载"<<MAX_COMPUTERS<<"台电脑<<<"<<endl;
}
//2.保存数据到文件
void saveData(){
    ofstream outFile(DATE_FILE);
    if(!outFile.is_open()){
        cout<<"错误：文件无法写入"<<endl;
        return;
    }
    //保存电脑记录
    for(int i=0;i<MAX_COMPUTERS;i++){
        outFile << computers[i].id << " "
                << computers[i].roomid << " "
                << computers[i].hang << " "
                << computers[i].lie << " "
                << computers[i].config << " "
                << computers[i].statu << " "
                << computers[i].historyCount << " "
                << computers[i].currentUser << " "
                << computers[i].currentId << " "
                << computers[i].loginTime << endl;
        //保留历史记录
        for(int h =0;h<computers[i].historyCount;h++){
            outFile << computers[i].history[h].studentName << " "
                    << computers[i].history[h].studenId << " "
                    << computers[i].history[h].time.startTime << " "
                    << computers[i].history[h].time.endTime << endl;
        }
    }
    //保留课表数据
    for(int r = 0;r<ROOM_COUNT;r++){
        outFile << rooms[r].id<<" "<<rooms[r].scheduleCount<<endl;
        for(int s=0;s<rooms[r].scheduleCount;s++){
            outFile << rooms[r].schedules[s].day<<" "
                    << rooms[r].schedules[s].period<<" "
                    << rooms[r].schedules[s].courseName<<" "
                    << rooms[r].schedules[s].teacherName<<endl;
        }
    }
    
    outFile.close();
    cout<<">>>数据已成功保存到："<<DATE_FILE<<endl;
}
//3.从文件加载到数据
void loadData(){
    ifstream inFile;
    if(!inFile.is_open()){
        cout<<">>>为找到文件默认初始化<<<"<<endl;
        initData();
        return ;
    }
    //加载计算机
    int idx = 0;
    while(idx<MAX_COMPUTERS && inFile >>computers[idx].id){
        inFile >> computers[idx].roomid >> computers[idx].hang >> computers[idx].lie;
        //inFile流无法直接接入string 和 枚举类型，所以需要进行类型转换
        string tempStr;
        inFile >>tempStr;
        computers[idx].config = tempStr;
        
        int temp = computers[idx].statu;
        inFile >> temp >> computers[idx].historyCount
               >> computers[idx].currentUser >> computers[idx].currentId >>computers[idx].loginTime;
        //加载历史
        for(int h=0 ; h<computers[idx].historyCount ;h++){
            inFile >> computers[idx].history[h].studentName
                   >> computers[idx].history[h].studenId
                   >> computers[idx].history[h].time.startTime
                   >> computers[idx].history[h].time.endTime;
            computers[idx].history[h].isValid = true;
        }
        idx++;
    }
    //加载课表
    int rCount = 0;
    while(rCount < ROOM_COUNT && inFile >> rooms[rCount].id){
        inFile >> rooms[rCount].scheduleCount;
        for(int r =0 ;  r<rooms[rCount].scheduleCount;r++){
            inFile >> rooms[rCount].schedules[r].day
                   >> rooms[rCount].schedules[r].period
                   >> rooms[rCount].schedules[r].courseName
                   >> rooms[rCount].schedules[r].teacherName;
            rooms[rCount].schedules[r].isSchedule =true;
        }
        rCount++;
    }
    inFile.close();
    cout << ">>> 数据已从 " << DATE_FILE << " 加载成功。" << endl;
}
// 4. 显示主菜单
void showMenu() {
    cout << "\n========================================" << endl;
    cout << "      计算机机房管理系统 (基础版)" << endl;
    cout << "========================================" << endl;
    cout << "1. 查看所有教室状态概览" << endl;
    cout << "2. 查询特定计算机详情" << endl;
    cout << "3. 学生上机登记" << endl;
    cout << "4. 学生下机登记" << endl;
    cout << "5. 管理排课信息 (添加/查看)" << endl;
    cout << "6. 设置计算机维修状态" << endl;
    cout << "0. 保存数据并退出" << endl;
    cout << "请输入选项：";
}
// 5. 查看教室概况
void viewOverview(){
    cout<<"\n---机房状态---"<<endl;
    //left 输出左对齐 setw()空几格
    cout << left << setw(6) << "教室" << setw(10) << "总机器" << setw(10) << "可用" << setw(10) << "使用中" << setw(10) << "维修" << endl;
    for(int r =1 ; r<=ROOM_COUNT;r++){
        int total = HANG_ROOM * LIE_ROOM ;
        int avail = 0, occupied = 0, maint = 0;
        //遍历所有电脑
        for(int i =0 ; i <MAX_COMPUTERS ;i++){
            if(computers[i].roomid == r){
                if(computers[i].statu == AVAILABLE) avail++;
                else if(computers[i].statu == OCCUPIED) occupied++;
                else if(computers[i].statu == MAINTENANCE) maint++;
            }
        }
        cout << setw(6) << r << setw(10) << total << setw(10) << avail << setw(10) << occupied << setw(10) << maint << endl;
    }
}
//6. 查看具体电脑的数据
void queryComputer(){
    int id;
    cout << "请输入计算机编号 (1-" << MAX_COMPUTERS << "): ";
    cin >> id;

    Computer* pc = findComputer(id);
    if(!pc){
        cout << "错误：无效的计算机编号！" << endl;
        return;
    }

    cout << "\n=== 计算机 #" << pc->id << " 详情 ===" << endl;
    cout << "位置：教室 " << pc->roomid << " - 第 " << pc->hang << " 排 - 第 " << pc->lie << " 列" << endl;
    cout << "配置：" << pc->config << endl;
    cout << "当前状态：" << getStatusStr(pc->statu) << endl;

    if(pc->statu == OCCUPIED){
        cout << "当前用户：" << pc->currentUser << " (学号:" << pc->currentId << ")" << endl;
        cout << "登录时间：" << pc->loginTime << endl;
    }

    cout << "\n--- 使用历史记录 (最近 " << pc->historyCount << " 条) ---" << endl;
    if(pc->historyCount == 0){
        cout<<"暂无记录"<<endl;
    }else{
        for(int i = 0 ;i < pc->historyCount ; i++){
            cout<<"["<<i+1<<"]"<<pc->history[i].studentName
                <<"("<<pc->history[i].studenId<<"):"
                <<pc->history[i].time.startTime<<"~"<<pc->history[i].time.endTime<<endl;
        }
    }
}
//7. 上机功能
void studentLogin(){
    int id;
    cout << "请输入要使用的计算机编号：";
    cin >> id;
    Computer* pc = findComputer(id);
    if (!pc) {
        cout << "错误：无效的计算机编号！" << endl;
        return;
    }
    if (pc->statu != AVAILABLE) {
        cout << "错误：该计算机当前不可用 (状态：" << getStatusStr(pc->statu) << ")" << endl;
        return;
    }
    
    string name,sid;
    cout << "请输入姓名：";
    cin >> name;
    cout << "请输入学号：";
    cin >> sid;

    pc->statu = OCCUPIED;
    pc->currentUser = name;
    pc->currentId = sid;
    pc->loginTime = getCurrentTime();

    cout << ">>> 登记成功！" << name << " 已开始使用计算机 #" << id << endl;
}
//8.下机功能
void studentLogout(){
    int id;
    cout << "请输入要释放的计算机编号：";
    cin >> id;
    Computer* pc = findComputer(id);
    if (!pc) {
        cout << "错误：无效的计算机编号！" << endl;
        return;
    }
    if (pc->statu != OCCUPIED) {
        cout << "错误：该计算机当前未被使用，无需下机！" << endl;
        return;
    }
    //添加历史使用记录
    if(pc->historyCount < MAX_HISTORY){
        int idx =pc->historyCount;
        pc->history[idx].studentName = pc->currentUser;
        pc->history[idx].studenId = pc->currentId;
        pc->history[idx].time.startTime = pc->loginTime;
        pc->history[idx].time.endTime = getCurrentTime(); // 模拟结束时间
        pc->history[idx].isValid = true;
        pc->historyCount++;
    }else{
        //此处可设置覆盖模式，从上到下
        int idx =pc->historyCount;
        cout<<"第"<<(idx)%MAX_HISTORY<<"被覆盖"<<endl;
        pc->history[idx].studentName = pc->currentUser;
        pc->history[idx].studenId = pc->currentId;
        pc->history[idx].time.startTime = pc->loginTime;
        pc->history[idx].time.endTime = getCurrentTime(); // 模拟结束时间
        pc->history[idx].isValid = true;
        pc->historyCount++;
    }
    // 重置状态
    pc->statu = AVAILABLE;
    pc->currentUser = "";
    pc->currentId = "";
    pc->loginTime = "";
    cout << ">>> 下机成功！计算机 #" << id << " 已释放。" << endl;
}
//9.排课管理
void manageSchedule() {
    int subMenu;
    cout << "\n--- 排课管理 ---" << endl;
    cout << "1. 添加新课表" << endl;
    cout << "2. 查看所有课表" << endl;
    cout << "请选择：";
    cin >> subMenu;

    if (subMenu == 1) {
        int rId, day, period;
        string course, teacher;
        cout << "请输入教室号 (1-6): "; cin >> rId;
        if (rId < 1 || rId > ROOM_COUNT) { cout << "无效教室号" << endl; return; }
        
        cout << "请输入星期 (1-5): "; cin >> day;
        cout << "请输入节次 (1-8): "; cin >> period;
        cout << "课程名称："; cin >> course;
        cout << "教师姓名："; cin >> teacher;

        // 检查冲突
        Room* room = &rooms[rId-1];
        bool conflict = false;
        for (int i = 0; i < room->scheduleCount; ++i) {
            if (room->schedules[i].day == day && room->schedules[i].period == period) {
                conflict = true;
                break;
            }
        }

        if (conflict) {
            cout << "错误：该时间段已有课程安排！" << endl;
        } else {
            if (room->scheduleCount < 40) {
                int idx = room->scheduleCount;
                room->schedules[idx].day = day;
                room->schedules[idx].period = period;
                room->schedules[idx].courseName = course;
                room->schedules[idx].teacherName = teacher;
                room->schedules[idx].isSchedule = true;
                room->scheduleCount++;
                cout << ">>> 排课成功！" << endl;
            } else {
                cout << "错误：该教室课表已满。" << endl;
            }
        }

    } else if (subMenu == 2) {
        int rId;
        cout << "输入要查询的教室号 (1-6, 0 表示所有): ";
        cin >> rId;

        for (int r = 0; r < ROOM_COUNT; ++r) {
            if (rId != 0 && (r + 1) != rId) continue;

            cout << "\n[教室 " << (r+1) << " 课表]" << endl;
            if (rooms[r].scheduleCount == 0) {
                cout << "  暂无课程。" << endl;
                continue;
            }
            cout << left << setw(6) << "星期" << setw(6) << "节次" << setw(15) << "课程" << setw(10) << "教师" << endl;
            for (int i = 0; i < rooms[r].scheduleCount; ++i) {
                cout << setw(6) << getDayStr(rooms[r].schedules[i].day)
                     << setw(6) << rooms[r].schedules[i].period
                     << setw(15) << rooms[r].schedules[i].courseName
                     << setw(10) << rooms[r].schedules[i].teacherName << endl;
            }
        }
    }
}
//10. 设置维修功能
void setMaintenance() {
    int id, statusVal;
    cout << "输入计算机编号："; cin >> id;
    Computer* pc = findComputer(id);
    if (!pc) { cout << "无效编号" << endl; return; }

    if (pc->statu == OCCUPIED) {
        cout << "警告：计算机正在使用中，不能设置为维修状态！" << endl;
        return;
    }

    cout << "输入状态 (0:恢复正常/可用，2:设为维修): ";
    cin >> statusVal;

    if (statusVal == 0) {
        pc->statu = AVAILABLE;
        cout << ">>> 已恢复为可用状态。" << endl;
    } else if (statusVal == 2) {
        pc->statu = MAINTENANCE;
        cout << ">>> 已标记为维修中。" << endl;
    } else {
        cout << "无效的状态值。" << endl;
    }
}

int main(){
     // 加载数据
    loadData();

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                viewOverview();
                break;
            case 2:
                queryComputer();
                break;
            case 3:
                studentLogin();
                break;
            case 4:
                studentLogout();
                break;
            case 5:
                manageSchedule();
                break;
            case 6:
                setMaintenance();
                break;
            case 0:
                saveData();
                cout << "感谢使用，再见！" << endl;
                return 0;
            default:
                cout << "无效选项，请重新输入。" << endl;
        }
    }

    return 0;
}
