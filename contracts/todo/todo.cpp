#include <todo.hpp>

void todo::get(uint64_t id)
{
    task_table task(_self, _self);

    auto itr = task.find(id);

    eosio_assert(itr != task.end(), "no task found for id");

    print("Account: ", name{itr->account}, " , ");
    print("Id: ", itr->id, " , ");
    print("Content: ", itr->content.c_str(), " , ");
}

void todo::create(const account_name account,
                  const string &content)
{
    require_auth(account);

    task_table task(_self, _self);

    auto itr = task.find(account);

    config conf;
    get_config(conf);

    uint64_t id = conf.id++;

    task.emplace(account, [&](auto &p) {
        p.id = id;
        p.account = account;
        p.content = content;
    });

    store_config(conf);

    print(name{account}, "Task created");
}

void todo::update(const account_name account,
                  const string &content,
                  uint64_t id)
{
    require_auth(account);

    task_table task(_self, _self);

    auto itr = task.find(account);

    eosio_assert(itr != task.end(), "no task found for accound");

    task.modify(itr, account, [&](auto &p) {
        p.content = content;
    });

    print(name{account}, " modified");
}

void todo::remove(const account_name account, uint64_t id)
{
    require_auth(account);

    task_table task(_self, _self);

    auto itr = task.find(id);

    eosio_assert(itr != task.end(), "Task not Found for Id");

    task.erase(itr);
    print(name{account}, "Task removed");
}
