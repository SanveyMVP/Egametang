#include "Event/Event.h"
#include "Event/EventConf.pb.h"

namespace Egametang {

Event::Event(NodeFactories& factories, EventConf& conf):
		action(NULL), condition(NULL)
{
	const ConditionConf& condition_conf = conf.condition();
	if (condition_conf.has_node())
	{
		const LogicNode& node_conf = condition_conf.node();
		BuildCondition(factories, node_conf, condition);
	}
}

void Event::BuildCondition(
		NodeFactories& factories, const LogicNode& conf,
		NodeIf*& node)
{
	int32 type = conf.type();
	node = factories.GetInstance(conf);
	for (int i = 0; i < conf.nodes_size(); ++i)
	{
		const LogicNode& logic_node_conf = conf.nodes(i);
		NodeIf* logic_node = NULL;
		BuildCondition(factories, logic_node_conf, logic_node);
		node->AddChildNode(logic_node, i);
	}
}

Event::~Event()
{
	delete condition;
	delete action;
}

void Event::Excute(ContexIf* contex)
{
	if(condition->Check(contex))
	{
		// 执行动作
		action->Excute(contex);
	}
}

int Event::Type() const
{
	return type;
}

} // namespace Egametang
