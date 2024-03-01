# UE NetWork

`各端包含元素`

以Server，ClientA，ClientB为例
* GameMode只包含在Server
* PlayerController_Server 只包含在Server(含有A,B的玩家控制器)
* 客户端只包含自己的玩家控制器
* GameState 三个端都有
* Pawn_Server,Pawn_A,Pawn_B 三个端都有
* PlayerState_Server,PlayerState_A,PlayerState_B 三个端都有
* GameInstance,UI 三个端都有独一无二不会复制到其他端

## Server - Client

### 网络信息传递的主要方法
* Replication 这是一个单向操作，只可以从服务端复制到客户端
* Rep_Notify Replication的特殊方法
* RPC 可以双向，还可以有很多其他操作

## Replication网络复制
Replication开启才会复制，不开启就不会复制。<br>
(Server->Client单向)Actor及其派生类和Component才有Replication<br>

类型:
* Actor Replication
* Property Replication
* Component Replication

### Actor Replication

* 服务端生成一个Replication Actor客户端也跟着生成
* 只有Actor Replication开启后，当前Actor所属的Property Replication，ComponentReplication和RPC才可使用(总开关)
```C++
bReplicates=true;//C++开启复制
Replicates 打勾//蓝图
```

### Property Replication

```C++
设置Replication//蓝图
//C++开启复制3步
1. 添加#include<Net/UnrealNetwork.h>
2. 属性前加UPROPERTY(Replicated)
3. 重写GetLifetimeReplicatedProps()//完整的就不写了太多了

```
条件复制

```C++
1. DOREPLIFETIME_CONDITION_NOTIFY()
2. COND_None：
   > COND_None 表示没有特殊的复制条件，即始终复制属性。属性的变化将在任何时候都同步到客户端。
   > 这是最常用的选项，适用于大多数需要同步的属性。例如，位置、生命值等经常需要在变化时同步到客户端的属性。
3. COND_InitialOnly：
   > 当属性使用 COND_InitialOnly 条件时，该属性只在初始化阶段进行复制。这意味着属性值将在 Actor 的生成和复制时传递给客户端，但在此后的变化将不再复制到客户端。
   > 这对于一些在角色生成时需要同步，但在游戏过程中不需要同步的属性是非常有用的。例如，一些初始设置，可能只需在角色生成时同步一次。
4. COND_OwnerOnly
   > 只将数据同步到拥有者，不会向其他代理同步
```
## Rep_Notify复制通知

蓝图中: 设置为Rep_Notify的变量在发生复制时，服务端和收到该值的客户端都可以调用一个自定义函数。<br>
C++中: 仅在客户端执行一个自定义函数	

```C++
设置Rep_Notify及自动生成//蓝图
//C++
1. 添加#include<Net/UnrealNetwork.h>
2. 属性前加UPROPERTY(ReplicatedUsing=OnRep_Var)//OnRep_Var被调用函数
3. 重写GetLifetimeReplicatedProps()//完整的就不写了太多了
4.UFUNTION()
  void OnRep_Var();//如果想让服务端也调用可在OnRep_Var内调用RPC函数

```

## Ownership所有权

* RPC需要确定哪个客户端将执行运行于客户端的RPC
* Actor复制与连接相关性
* 在涉及所有者时的Actor属性复制条件

> * 连接所有权对于 RPC 这样的机制至关重要，因为当您在 actor 上调用 RPC 函数时，除非 RPC 被标记为多播，
否则就需要知道要在哪个客户端上执行该 RPC，它可以查找所属连接来确定将RPC发送到哪条连接
> * 连接所有权会在 actor 复制期间使用，用于确定各个连接上有哪些 actor 获得了更新。
对于那些将 bOnlyRelevantToOwner 设置为 true 的 actor，只有拥有此 actor 的连接才会接收这个 actor 的属性更新。
默认情况下，所有 PlayerController 都设置了此标志，正因如此，客户端才只会收到它们拥有的 PlayerController 的更新。
这样做是出于多种原因，其中最主要的是防止玩家作弊和提高效率。
> * 条件复制需要用到，服务器权威Actor，客户端自主代理Actor，客户端模拟Actor

设置获取拥有者：<br>
SpawnActor,SetOwner,Possess,Unpossess,GetOwner

## Actor Role
这里说的是玩家

* Authority 服务端的玩家都是Authority
* Simulated Proxy 客户端对别人的代理，仅模拟没有自主权，在同步时不能做到每一帧都同步，代理会通过插值模拟
* Autonomous Proxy 客户端对自己的代理

Role 的获取，本地的和远端的

## RPC远程过程调用

### 三种基本通信
1. 客户端调用运行在服务端的自定义事件（事件replicates属性设置为RunOnServer）从而与服务器端通讯
1. 服务器调用运行在客户端的自定义事件（事件replicates属性设置为RunOnOwningClient）从而与客户端通讯
1. 服务器调用广播事件（事件replicates属性设置为Multicast）与所有客户端通讯
>不可以有返回值，默认不可靠（使他可靠Reliable）

<img src="MarkdownImage\\RPCFromServerCalled.png">
<img src="MarkdownImage\\RPCFromClientCalled.png">

```C++
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerNormalSpeedWalk();
	void ServerNormalSpeedWalk_Implementation();
	bool ServerNormalSpeedWalk_Validate();
```
```C++
//RPC操作
void AX_Sleepy_BaseCharacter::X_SpringSpeedWalk(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600;//自己先做，这句话的目的是因为网络有延迟，为了显得游戏流畅
	this->ServerSpringSpeedWalk();//提交服务器，服务器会验证后叫客户端执行MaxWalkSpeed = 600
}

void AX_Sleepy_BaseCharacter::ServerNormalSpeedWalk_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;
}
```


