//
// Created by Saleh on 01/06/2022.
//

#include "MultiStructures.h"


namespace ULIFB
{

    void MultiStructures::decreaseNumOfEmployeesWithNoSalary()
    {
        this->number_of_employees_with_no_salary--;
    }

    void MultiStructures::decreaseSumOfGradesEmployeesWithNoSalary(long long int decrease)
    {
        this->sum_of_grades_for_emp_with_no_salary -= decrease;
    }

    void MultiStructures::increaseSumOfGradesEmployeesWithNoSalary(long long int increase)
    {
        this->sum_of_grades_for_emp_with_no_salary += increase;
    }

    void MultiStructures::addEmployee(SalaryID sal_id,shared_ptr<Employee> emp_to_add)
    {
        employees.addElement(emp_to_add->getEmployeeID(), emp_to_add);
        number_of_employees_with_no_salary++;
        sum_of_grades_for_emp_with_no_salary += emp_to_add->getEmployeeGrade();
        total_num_of_employees++;
    }

    shared_ptr<Employee> MultiStructures::findEmployee(int emp_id)
    {
        // complexity is O(1) on average!
        Node<long long int,shared_ptr<Employee>>* emp_to_find =  this->employees.Find(emp_id);
        if (emp_to_find != nullptr)
        {
            return(*(emp_to_find->data)); // returns the data if the emp was found!
        }
        return nullptr; // emp was not found!
    }

    void MultiStructures::removeEmployee(int emp_id)
    {
        // complexity is O(log(n))!
        shared_ptr<Employee> emp_to_find = this->findEmployee(emp_id);
        if (emp_to_find == nullptr)
        {
            return;
        }
        
        if (emp_to_find->getEmployeeSalary() == 0) //different case depending on the employee's salary!
        {
            this->number_of_employees_with_no_salary--;
            this->sum_of_grades_for_emp_with_no_salary -= emp_to_find->getEmployeeGrade();
            employees.deleteElement(emp_id);
            this->total_num_of_employees--; // saleh, added --
            return;
        }
        // else if salary is bigger than zero remove him from the tree!
        employees_with_salary.removeNode(emp_to_find->getEmployeeSalaryID());
        employees.deleteElement(emp_id);
        this->total_num_of_employees--; // saleh, added --
    }

    void MultiStructures::IncreaseEmployeeSalary(int emp_id, long long int increase)
    {
        // complexity is O(log(n))!
        shared_ptr<Employee> emp_to_find =  this->findEmployee(emp_id);
        if (emp_to_find == nullptr)
        {
            return; // should never get here!
        }
        if (emp_to_find->getEmployeeSalary() == 0)
        {
            this->number_of_employees_with_no_salary--;
            this->sum_of_grades_for_emp_with_no_salary-=emp_to_find->getEmployeeGrade();
            emp_to_find->increaseSalary(increase);
            this->employees_with_salary.addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
            return;
        }
        else if (emp_to_find->getEmployeeSalary() > 0)
        {
            this->employees_with_salary.removeNode(emp_to_find->getEmployeeSalaryID()); // remove old node!
            emp_to_find->increaseSalary(increase);
            this->employees_with_salary.addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
        }
        
    }

    void MultiStructures::promoteEmployee(int emp_id, long long int bump)
    {
        // saleh's code to test other functions - remove later
        // complexity is O(log(n))!
        if (bump <= 0)
        {
            return; // there's nos increase to perform!
        }
        
        shared_ptr<Employee> emp_to_find =  this->findEmployee(emp_id);
        if (emp_to_find == nullptr)
        {
            return; // should never get here!
        }
        if (emp_to_find->getEmployeeSalary() == 0)
        {
            this->sum_of_grades_for_emp_with_no_salary -= emp_to_find->getEmployeeGrade();
            emp_to_find->bumpGrade(bump);
            this->sum_of_grades_for_emp_with_no_salary += emp_to_find->getEmployeeGrade();
            return;
        }
        if (emp_to_find->getEmployeeSalary() > 0)
        {
            this->employees_with_salary.removeNode(emp_to_find->getEmployeeSalaryID()); // remove old node!
            emp_to_find->bumpGrade(bump);
            this->employees_with_salary.addNode( emp_to_find->getEmployeeSalaryID() , emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
        }
    }

    long long int MultiStructures::getSonsSumOnly(tree_node<SalaryID,shared_ptr<Employee>> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        
        return(node->sons_sum - node->sum);
        
    }

    long long int MultiStructures::getSonsNumOnly(tree_node<SalaryID,shared_ptr<Employee>> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return(node->sons_num - 1);
        
    }

    long long int MultiStructures::getNodeSumInSubTree(tree_node<SalaryID,shared_ptr<Employee>> * node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return node->sons_sum;
    }

    long long int MultiStructures::getNodeNumInSubTree(tree_node<SalaryID,shared_ptr<Employee>> * node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return node->sons_num;
    }

    void MultiStructures::getSumOfGradesAux(tree_node<SalaryID,shared_ptr<Employee>> * node,long long int *sum, long long int *num)
    {
        // slightly modified solution, maybe wrong!
        // if (node == nullptr || (*num) == 0)
        // {
        //     return;
        // }
        // if ( ( (*num) - ( node->sons_num - getNodeNumInSubTree(node->left_son) ) ) < 0 ) // counted too many nodes
        // {
        //    return getSumOfGradesAux(node->right_son,sum,num);
        // }

        // if ( (*num) > 0 )
        // {
        //     (*num) -= ( node->sons_num - getNodeNumInSubTree(node->left_son) );
        //     (*sum) += ( node->sons_sum - getNodeSumInSubTree(node->left_son) );            
        //     return getSumOfGradesAux(node->left_son,sum,num);
        //     // original
        // }
        
        // if ( (*num) < 0 )
        // {
        //     (*num) += ( node->sons_num - getNodeNumInSubTree(node->left_son) );
        //     (*sum) -= ( node->sons_sum - getNodeSumInSubTree(node->left_son) );            
        //     return getSumOfGradesAux(node->right_son,sum,num);
        //     // added
        // }

        if (node == nullptr || (*num) == 0)
        {
            return;
        }
        if ( ( (*num) - ( node->sons_num - getNodeNumInSubTree(node->left_son) ) ) < 0 ) // counted too many nodes
        {
           return getSumOfGradesAux(node->right_son,sum,num);
        }
        (*num) -= ( node->sons_num - getNodeNumInSubTree(node->left_son) );
        (*sum) += ( node->sons_sum - getNodeSumInSubTree(node->left_son) );            
        return getSumOfGradesAux(node->left_son,sum,num);
        
    }

    tree_node<SalaryID,shared_ptr<Employee>> *MultiStructures::findNodeWithSufficientSons(tree_node<SalaryID,shared_ptr<Employee>> * node,long long int num_of_top_workers)
    {
        // start from the maximum and go up!
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->sons_num >= num_of_top_workers)
        {
            return node;
        }
        if (node->sons_num < num_of_top_workers)
        {
            return findNodeWithSufficientSons(node->parent,num_of_top_workers);    
        }
        //shouldnt get here
        return nullptr;        
    }

    long long int MultiStructures::SumOfBumpGradeBetweenTopWorkersByGroup(long long int number_of_top_workers)
    {
        // complexity is O(log*(k) + log(n))  

        long long int sum = 0;
        long long int num = 0;
        // start the search from the most right node!
        tree_node<SalaryID,shared_ptr<Employee>> *current = findNodeWithSufficientSons(this->employees_with_salary.max,number_of_top_workers);
        if (current == nullptr)
        {
            return -1; // the number of top workers is too big, we don't have that many workers!
        }

        num = number_of_top_workers;
        sum = 0;
        getSumOfGradesAux(current,&sum,&num);
        current = findNodeWithSufficientSons(this->employees_with_salary.max,number_of_top_workers);// to remove
        num = number_of_top_workers; // to remove
        sum = 0;// to remove
        getSumOfGradesAux(current,&sum,&num); // to remove
        return sum;
    }

    hash_table<shared_ptr<Employee>>* MultiStructures::getEmployesHashTable()
    {
        return(&(this->employees));
    }

    AVL_Tree<SalaryID,shared_ptr<Employee>>* MultiStructures::getEmployessWithSalaryTree()
    {
        return(&(this->employees_with_salary));
    }

    void MultiStructures::MoveTreeToArray(shared_ptr<Employee> * array, tree_node<SalaryID, shared_ptr<Employee> >* node, long long int* counter_ptr, long long int num)
    {
        if (node == nullptr || *counter_ptr == num ) {
            return;
        }
        /* first recur on left child */
        MoveTreeToArray(array, node->left_son, counter_ptr, num);
        // added by - saleh
        if (*counter_ptr == num ) {
            return;
        }
        if (node->data)
        {
            array[*counter_ptr] = *(node->data);
            (*counter_ptr)++;
        }
        else
        {
            return;
        }
        if (*counter_ptr == num) {
            return;
        }
        /* now recur on right child */
        MoveTreeToArray(array, node->right_son, counter_ptr, num);
    }

    void MultiStructures::updateHeight(tree_node<SalaryID,shared_ptr<Employee>> *node) 
    {
        if (node->left_son == nullptr && node->right_son == nullptr) {
            node->height = 1;
            return;
        }
        if (node->left_son == nullptr) {
            updateHeight(node->right_son);
            node->height = node->right_son->height + 1;
        } else if (node->right_son == nullptr) {
            updateHeight(node->left_son);
            node->height = node->left_son->height + 1;
        } else {
            updateHeight(node->right_son);
            updateHeight(node->left_son);
            node->height = std::max(node->right_son->height , node->left_son->height) + 1;
        }
    }

    void MultiStructures::updateNum(tree_node<SalaryID,shared_ptr<Employee>> *node) 
    {
        if (node->left_son == nullptr && node->right_son == nullptr) {
            node->sons_num = 1;
            return;
        }
        if (node->left_son == nullptr) {
            updateNum(node->right_son);
            node->sons_num = node->right_son->sons_num + 1;
        } else if (node->right_son == nullptr) {
            updateNum(node->left_son);
            node->sons_num = node->left_son->sons_num + 1;
        } else {
            updateNum(node->right_son);
            updateNum(node->left_son);
            node->sons_num = node->right_son->sons_num + node->left_son->sons_num + 1;
        }
    }

    void MultiStructures::updateSum(tree_node<SalaryID,shared_ptr<Employee>> *node) 
    {
        if (node->left_son == nullptr && node->right_son == nullptr) {
            node->sons_sum = node->sum;
            return;
        }
        if (node->left_son == nullptr) {
            updateSum(node->right_son);
            node->sons_sum = node->right_son->sons_sum + node->sum;
        } else if (node->right_son == nullptr) {
            updateSum(node->left_son);
            node->sons_sum = node->left_son->sons_sum + node->sum;
        } else {
            updateSum(node->right_son);
            updateSum(node->left_son);
            node->sons_sum = node->right_son->sons_sum + node->left_son->sons_sum + node->sum;
        }
    }

    void MultiStructures::UpdataRankTreeFields(tree_node<SalaryID, shared_ptr<Employee>>* root)
    {
        if (root == nullptr)
        {
            return;
        }
        this->employees_with_salary.treeClear(); // updating the tree to be the new one!
        this->employees_with_salary.root = root;
        this->employees_with_salary.max = root->findMax();
        this->employees_with_salary.min = root->findMin();
        updateHeight(root);
        updateNum(root);
        updateSum(root);
    }

    void MultiStructures::MergeHashTable(hash_table<shared_ptr<Employee>> *other) // saleh
    {
        LList<long long int,shared_ptr<Employee>>* current_list = other->arr[0];
        for (long long int i = 0; i < other->arr_size; i++) // maybe arr_size+1
        {
            current_list = other->arr[i];
            Node<long long int, shared_ptr<Employee>>* current_node = current_list->head->next;
            while (current_node != current_list->tail)
            {
                this->getEmployesHashTable()->addElement(current_node->key,*(current_node->data));
                current_node = current_node->next;
            }
        }
    }

    void MultiStructures::UpdateOwnerIdInHash(int new_owner) // saleh
    {
        LList<long long int,shared_ptr<Employee>>* current_list = this->employees.arr[0];
        for (int i = 0; i < this->employees.arr_size; i++) // maybe arr_size+1
        {
            current_list = this->employees.arr[i];
            Node<long long int, shared_ptr<Employee>>* current_node = current_list->head->next;
            while (current_node != current_list->tail)
            {
                current_node->data->get()->UpdateCompanyID(new_owner);
                current_node = current_node->next;
            }
        }
    }

    void MultiStructures::TraverseOnTree(tree_node<SalaryID,shared_ptr<Employee>>* current)
    {
        if (current == nullptr)
        {
            return;
        }
        current->sum = current->data->get()->getEmployeeGrade();
        TraverseOnTree(current->left_son);
        TraverseOnTree(current->right_son);
        
    }

    // long long int MultiStructures::countNodes(tree_node<SalaryID, shared_ptr<Employee>>* node)
    // {
    //     if( node == nullptr)
    //     {
    //         return 0;
    //     }
    //     return (countNodes(node->left_son) + countNodes(node->right_son) + 1);
    // }

    void MultiStructures::MergeStructures(MultiStructures *other, int new_owner)
    {
        // numbers merge
        this->number_of_employees_with_no_salary += other->number_of_employees_with_no_salary;
        this->sum_of_grades_for_emp_with_no_salary += other->sum_of_grades_for_emp_with_no_salary;

        // hashtable merge
        int hash_elements = other->employees.getNumberOfElements();
        if (hash_elements == 0)
        {
            other->getEmployesHashTable()->DeleteHashTable();
            other->getEmployeesWithSalaryTree()->treeClear();
            return; // there's nothing to merge!
        }
        
        this->MergeHashTable(other->getEmployesHashTable());
        this->UpdateOwnerIdInHash(new_owner);
        other->getEmployesHashTable()->DeleteHashTable();
        
        //tree merge
        long long int acquirer_num = this->employees_with_salary.size;
        long long int target_num = other->employees_with_salary.size;
        long long int after_num = acquirer_num + target_num;
        if (target_num == 0)
        {
            other->getEmployeesWithSalaryTree()->treeClear(); // saleh
            return; // there's nothing to merge!
        }
        AVL_Tree<SalaryID, shared_ptr<Employee>>* acquirer_employee_tree = this->getEmployessWithSalaryTree();
        AVL_Tree<SalaryID, shared_ptr<Employee>>* target_employee_tree = other->getEmployessWithSalaryTree();

        shared_ptr<Employee> *acquirer_employees_arr = new shared_ptr<Employee>[acquirer_num]();
        shared_ptr<Employee> *target_employees_arr = new shared_ptr<Employee>[target_num]();

        long long int counter = 0;
        long long int *counter_ptr = &counter;
        MoveTreeToArray(acquirer_employees_arr, acquirer_employee_tree->root, &counter, acquirer_num);
        *counter_ptr = 0;
        MoveTreeToArray(target_employees_arr, target_employee_tree->root, &counter, target_num);
        counter_ptr = nullptr;

        shared_ptr<Employee> *new_arr = new shared_ptr<Employee>[after_num]();
        // merge_func should be adjusted for the new rank_tree!
        merge_func(target_employees_arr, acquirer_employees_arr, target_num, acquirer_num, new_arr);
        tree_node<SalaryID, shared_ptr<Employee>>* new_tree = createTreeFromSortedArrAuxForSalary(new_arr, 0, acquirer_num+target_num-1,nullptr);
        // doesn't update fields correctly! ----- saleh!
        TraverseOnTree(new_tree);
        this->UpdataRankTreeFields(new_tree);
        this->employees_with_salary.size = after_num;

        delete[] acquirer_employees_arr;
        delete[] target_employees_arr;
        delete[] new_arr;
        
    }

    tree_node<SalaryID,shared_ptr<Employee>> *MultiStructures::findSalaryNode(tree_node<SalaryID,shared_ptr<Employee>>* current,long long int Salary)
    {
        if (current == nullptr)
        {
            return nullptr;
        }

        if (Salary == current->key->getSalary())
        {
            return current;
        }
        
        else if (Salary > current->key->getSalary())
        {
            return findSalaryNode(current->left_son,Salary);
        }

        else if (Salary < current->key->getSalary())
        {
            return findSalaryNode(current->right_son,Salary);
        }
        //shouldnt get here
        return nullptr;
        
    }

    tree_node<SalaryID,shared_ptr<Employee>> *MultiStructures::findLCA(tree_node<SalaryID,shared_ptr<Employee>> *root, SalaryID MinBound, SalaryID MaxBound)
    {
        // Base case
        if (root == nullptr)
        {
            return nullptr;
        }

        // If either n1 or n2 matches with root's key, report
        // the presence by returning root (Note that if a key is
        // ancestor of other, then the ancestor key becomes LCA
        if ( (root->key->getSalary()) == MinBound.getSalary() || (root->key->getSalary()) == MaxBound.getSalary())
            return root;

        // Look for keys in left and right subtrees
        tree_node<SalaryID,shared_ptr<Employee>> *left_lca  = findLCA(root->left_son, MinBound, MaxBound);
        tree_node<SalaryID,shared_ptr<Employee>> *right_lca  = findLCA(root->right_son, MinBound, MaxBound);

        // If both of the above calls return Non-NULL, then one key
        // is present in once subtree and other is present in other,
        // So this node is the LCA
        if (left_lca && right_lca)
        {
            return root;
        }
        // Otherwise check if left subtree or right subtree is LCA
        return (left_lca != nullptr)? left_lca : right_lca;
        
    }

    void MultiStructures::AdjustSummation(tree_node<SalaryID,shared_ptr<Employee>> *node,SalaryID minbound, SalaryID maxbound,long long int *sum_ptr, long long int *num_ptr)
    {
        if (node == nullptr)
        {
            return;
        }

        if ( (*node->key) >= minbound && (*node->key) <= maxbound )
        {
            (*sum_ptr) += node->sons_sum;
            (*num_ptr) += node->sons_num;
        }

        tree_node<SalaryID,shared_ptr<Employee>> *current = node;
        // seperate to 2 different cases:
        if ( (*node->key) < minbound )
        {
            (*sum_ptr) -= node->sons_sum;
            (*num_ptr) -= node->sons_num;
            while ( current != nullptr )
            {
                if ( (*current->key) >= minbound )
                {
                    AdjustSummation(current,minbound,maxbound,sum_ptr,num_ptr);
                }
                current = current->right_son;
            }
        }

        if ( (*node->key) > maxbound )
        {
            (*sum_ptr) -= node->sons_sum;
            (*num_ptr) -= node->sons_num;
            while ( current != nullptr )
            {
                if ( (*current->key) <= maxbound )
                {
                    AdjustSummation(current,minbound,maxbound,sum_ptr,num_ptr);
                }   
                current = current->left_son;
            }
        }
    }

    long double MultiStructures::AverageBumpGradeBetweenSalaryByGroup(int lowerSalary, int higherSalary)
    {
        long long int num_of_employees_in_range = 0;
        long long int sum_of_employees_grade_in_range = 0;
        if ( lowerSalary == 0 )
        {
            num_of_employees_in_range += this->number_of_employees_with_no_salary;
            sum_of_employees_grade_in_range += this->sum_of_grades_for_emp_with_no_salary;
        }
        SalaryID min_sal = SalaryID(lowerSalary,0); // (lowerSalary > 0) ? SalaryID(lowerSalary,0) : this->employees_with_salary.min->data->get()->getEmployeeSalaryID();
        SalaryID max_sal = SalaryID(higherSalary,0);
        bool added_min = false;
        bool added_max = false;
        tree_node<SalaryID,shared_ptr<Employee>> *min_node = findSalaryNode(this->employees_with_salary.root,lowerSalary);
        tree_node<SalaryID,shared_ptr<Employee>> *max_node = findSalaryNode(this->employees_with_salary.root,higherSalary);
        shared_ptr<Employee> min_emp = make_shared<Employee>(0,0,min_sal.getID(),min_sal.getSalary());
        shared_ptr<Employee> max_emp = make_shared<Employee>(0,0,max_sal.getID(),max_sal.getSalary());
        if (min_node == nullptr)
        {
            this->employees_with_salary.addNode(min_sal,min_emp,min_emp->getEmployeeGrade());
            min_node = this->employees_with_salary.findNode(min_sal);
            added_max = true;
        }
        if (max_node == nullptr)
        {
            this->employees_with_salary.addNode(max_sal,max_emp,max_emp->getEmployeeGrade());
            max_node = this->employees_with_salary.findNode(max_sal);
            added_max = true;
        }

        // perform actual calculations!
        tree_node<SalaryID,shared_ptr<Employee>>* LCA_node = findLCA(this->employees_with_salary.root,min_sal,max_sal);
        AdjustSummation(LCA_node,min_sal,max_sal,&sum_of_employees_grade_in_range,&num_of_employees_in_range);
        // findAverageParameters(LCA_node,min_node,max_node,&sum_of_employees_grade_in_range,&num_of_employees_in_range);

        if (added_min == true)
        {
            this->employees_with_salary.removeNode(min_sal);
        }
        if (added_max == true)
        {
            this->employees_with_salary.removeNode(max_sal);
        }
    
        if (num_of_employees_in_range == 0)
        {
            return (-1);
        }
        return((sum_of_employees_grade_in_range)/(num_of_employees_in_range));
        // find the employee with lowest salary and highest salary in range!
    }


    //********************************************helper_Functions_for_Acquire***********************************
    void MultiStructures::merge_func(shared_ptr<Employee> arr1[], shared_ptr<Employee> arr2[] ,int n1, int n2,
                              shared_ptr<Employee>  arr3[])
    {
        int i = 0, j = 0, k = 0;

        while (i<n1 && j <n2)
        {
            // previously, didnt work for some reason!
            if ( (arr1[i]->getEmployeeSalaryID()) < (arr2[j]->getEmployeeSalaryID()) )//ommmmm
                arr3[k++] = arr1[i++];
            else
                arr3[k++] = arr2[j++];
            // SalaryID sal_id1 = (arr1[i]->getEmployeeSalaryID());
            // SalaryID sal_id2 = (arr2[j]->getEmployeeSalaryID());
            // if ( sal_id1 < sal_id2 )//ommmmm
            //     arr3[k++] = arr1[i++];
            // else
            //     arr3[k++] = arr2[j++];
        }

        // Store remaining elements of first array
        while (i < n1)
            arr3[k++] = arr1[i++];

        // Store remaining elements of second array
        while (j < n2)
            arr3[k++] = arr2[j++];
    }

    tree_node<SalaryID, shared_ptr<Employee>>* MultiStructures::createTreeFromSortedArrAuxForSalary(shared_ptr<Employee> array[], long long int start,
                                                                                         long long int end, tree_node<SalaryID, shared_ptr<Employee>> *parent)
    {
        if(start > end)
            return nullptr;
        long long int mid = (start+end)/2;
        tree_node<SalaryID, shared_ptr<Employee>> *new_node = new tree_node<SalaryID, shared_ptr<Employee>>(array[mid]->getEmployeeSalaryID(),array[mid],parent, nullptr, nullptr, 0);
        new_node->left_son = createTreeFromSortedArrAuxForSalary(array, start, mid-1, new_node);
        new_node->right_son = createTreeFromSortedArrAuxForSalary(array, mid+1, end, new_node);
        new_node->updateHeight();
        return new_node;
    }

    void MultiStructures::bumpGradeForSumOfGradesForEmpWhithNoSalary(long long int bumpGrade){
        sum_of_grades_for_emp_with_no_salary+=bumpGrade;
    }

    AVL_Tree<SalaryID,shared_ptr<Employee>>* MultiStructures::getEmployeesWithSalaryTree(){
        return &employees_with_salary;
    }

    long long int MultiStructures::getNumOfEmployeesWithNoSalary(){
        return number_of_employees_with_no_salary;
    }

    long long int MultiStructures::getSumOfGradesForEmployeesWithNoSalary(){
        return sum_of_grades_for_emp_with_no_salary;
    }

} // namespace ULIFB
