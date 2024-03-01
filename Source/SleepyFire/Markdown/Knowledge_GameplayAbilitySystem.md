# Gameplay Ability System
`Replication Mode`

Replication Mode|Use Case|Description
----------------|--------|-----------
Full | Single Player | Gameplay Effects are replicated to all clients.
Mixed | Multiplayer,Player-Controlled | Gameplay Effects are replicated to the owning client only.Gameplay Cues and Gameplay Tags replicated to all clients.
Minimal | Multiplayer,AI-Controlled | Gameplay Effects are not replicated.Gameplay Cues and Gameplay Tags are replicated to all clients

> Mixed：拥有该组件的Actor的Owner必须是Controller，Pawn 或 character会在Possess函数中自动设置
  Owner为Controller，PlayerState默认设置Owner是Controller。其他Actor需要手动调用SetOwner()到
  Controller.
允许预测，客户端可以直接更改属性，服务器可以回滚无效的更改

# Gameplay Effect

`Duration Policy`

Duration Policy | Describe
----------------|---------
Instant|立即永久改变Attribute的BaseValue.GameplayTags将不适用，即使一帧也不行。
Duration|Add & Remove用于临时修改Attribute的CurrentValue，并且添加GameplayTags（ 在GameplayEffect过期时将会被删除或者手动删除）。
Infinite|Add & Remove用于临时修改Attribute的CurrentValue，并且添加GameplayTags（在GameplayEffect被移除时删除）。永不过时，必须通过Ability或ASC手动删除。
Duration+Periodic|每隔x秒周期性的执行Modifiers 和Executions.周期性效果可以看作是Instant GameplayEffects，每次修改属性的BaseValue并且执行GameplayCues。

`Attribute Change Pre Or Post`

函数|目的
----|----
PreAttributeChange / PreAttributeBaseChange | 这些函数会在属性被修改前受到调用。它们用于强制实施与属性的值有关的规则，例如"生命值必须介于0和最大生命值之间"，而且不应触发针对属性更改的游戏中反应。
PreGameplayEffectExecute|在修改属性的值之前，此函数可拒绝或改变提议的修改。也就是说PreAttributeChange限制值的范围，而本函数用于是否真正修改值。
PostGameplayEffectExecute|在修改属性的值之后，此函数可立即对更改作出反应。这通常包括限制属性的最终值，或者触发针对新值的游戏中反应，例如，当"生命值"属性降到零时死去。
> *PreGameplayEffectExecute
  *PreAttributeBaseChange
  *PreAttributeChange
  *PostAttributeChange
  *PostGameplayEffectExecute
  *

`结构体`

Struct | Content
-------|--------
FGameplayEffectModCallbackData | 
FGameplayEffectSpec | 


## Stacking Gameplay Effects

`游戏效果堆叠:堆叠只能用于Duration和Infinite类型的GameplayEffects。`

`StackingType`

StackingType|效果
------------|----
None|不会叠加，如果多次释放则每个Effect相当于单个Effect
AggregateBySource|在Effect源堆叠，同种类型Effect有堆叠上限
AggregateByTarget|在目标处堆叠，不论Effect是否同类型，都会限制堆叠上限


	StackLimitCount：可叠加的最大层数
	如果连续Apply该Effect则只有小于StackLimitCount 的Effect的Stack是有效的
	Eg:这里我设置最大值为3
	当我连续调用四次该Effect时，只有前三个有效，最后一个无效
	同时，在上图中Stacks为1，代表增加一层Effect，如果为2则表示增加两层Effect，
	多出堆栈的Apply在设置StackDurationRefreshPolicy后是可以刷新Duration
	每层Effect如果是Modifiers来计算则为直接叠加的效果，
	比如用Modifiers来增加3攻击力，则第一层为增加3攻击力，
	则第二层为增加6攻击力，则第三层为增加9攻击力，
	而如果需要根据层数不同而改变增加的值，则需要使用Executions。

`Policy`

Policy|效果
------|----
StackDurationRefreshPolicy|每次ApplyEffect时是否刷新Effect的持续时间，即使层数不增加也会刷新Duration
StackPeriodResetPolicy|每次ApplyEffect时是否刷新周期时间（Period），即使层数不增加也会刷新Period
StackExpirationPolicy|当Effect的Duration结束时，层数的结束方式，三种类型ClearEntireStack、RemoveSingleStackAndRefreshDuration、RefreshDuration
ClearEntireStack|Duration结束时清楚所有层数
RemoveSingleStackAndRefreshDuration|Duration结束时减少一层，然后重新经历一个Duration，一直持续到层数减为0
RefreshDuration|Duration结束时再次刷新Duration，这相当于无限Duration，可以通过调用void FActiveGameplayEffectsContainer::OnStackCountChange(FActiveGameplayEffect& ActiveEffect, int32 OldStackCount, int32 NewStackCount)来处理层数，可以达到Duration结束时减少两层并刷新Duration这样复杂的效果。

# GameplayTag
 
* instance的Effect不会有Tag
* 使用Stack后，添加的Tag将会被标记为1个
* 不使用Stack时，同一个Tag将会有个数标明


