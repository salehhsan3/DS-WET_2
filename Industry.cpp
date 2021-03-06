//
// Created by Saleh on 01/06/2022.
//

#include "Industry.h"

namespace ULIFB
{

    void mergeGroupData(Company* new_root, Company* old_root)
    {
        new_root->getEmployeesMultiStructures()->MergeStructures(old_root->getEmployeesMultiStructures(), new_root->getCompanyId() );
    }

    int Industry::getNumberOfCompanies()
    {
        return
        (this->number_of_companies);
    }

    MultiStructures* Industry::getEmployeesMultiStructures()
    {
        return(&(this->employees));
    }

    StatusType Industry::AddEmployee(int employeeID, int companyID, int grade)
    {
        // saleh's implementation - to remove later!
        if ( (employeeID <= 0) || (companyID <= 0) |( companyID > this->getNumberOfCompanies()) || (grade < 0) )
        {
            return INVALID_INPUT;
        }
        shared_ptr<Employee> emp_to_find =  this->getEmployeesMultiStructures()->findEmployee(employeeID);
        if (emp_to_find != nullptr)
        {
            return FAILURE;
        }

        shared_ptr<Employee> emp_to_add = make_shared<Employee>(employeeID,companyID,grade,0);
        this->getEmployeesMultiStructures()->addEmployee(SalaryID(0,employeeID),emp_to_add);
        // employee's company has to exist!
        this->companies_union.find(companyID)->data->getEmployeesMultiStructures()->addEmployee(SalaryID(0,employeeID),emp_to_add);
        if (max_id < employeeID){
            max_id = employeeID;
        }
        return SUCCESS;
    }

    StatusType Industry::RemoveEmployee(int employeeID)
    {
        // complexity is O(log(n)) on average
        if (employeeID <= 0)
        {
            return INVALID_INPUT;
        }
        shared_ptr<Employee> emp_to_find =  this->getEmployeesMultiStructures()->findEmployee(employeeID);
        if (emp_to_find == nullptr)
        {
            return FAILURE;
        }
        
        int company_id = emp_to_find->getEmployersid();
        this->companies_union.find(company_id)->data->getEmployeesMultiStructures()->removeEmployee(employeeID); // remove from company
        this->getEmployeesMultiStructures()->removeEmployee(employeeID); // remove from Industry
        return SUCCESS;
    }

    StatusType Industry::AcquireCompany(int companyID1, int companyID2, double factor)
    {
        // saleh's implementation - remove later!
        if (companyID1 <= 0 || (companyID1 > this->getNumberOfCompanies()) 
            || companyID2 <=0 || (companyID2 > this->getNumberOfCompanies())
            || factor <= 0)
        {
            return INVALID_INPUT;
        }

        Company* acquirer = this->companies_union.find(companyID1)->data;
        Up_Tree_node<long long int,Company*> *target_node = this->companies_union.find(companyID2);
        Company* target = this->companies_union.find(companyID2)->data;
        if (acquirer == target)
        {
            return INVALID_INPUT;
        }

        long double value_increase = ( target->getEnterpriseValue() * factor); 
        Up_Tree_node<long long int,Company*> *new_root = this->companies_union.UnionGroups(companyID1,companyID2,value_increase);    
        Company* new_owner = this->companies_union.find(companyID1)->data; // this is the root of the group!
        if (new_root != target_node)
        {
            new_owner->IncreaseCompanyValue(value_increase);
        }
        if (new_owner->getCompanyId() == acquirer->getCompanyId() )
        {
            new_owner->getEmployeesMultiStructures()->MergeStructures(target->getEmployeesMultiStructures(),new_owner->getCompanyId());
        }
        else
        {
            new_owner->getEmployeesMultiStructures()->MergeStructures(acquirer->getEmployeesMultiStructures(),new_owner->getCompanyId());
        }
        return SUCCESS;
    }

    StatusType Industry::EmployeeSalaryIncrease(int employeeID, int salaryIncrease)
    {
        // complexity is O(log(n))! on average
        if (employeeID <= 0  || salaryIncrease <= 0)
        {
            return INVALID_INPUT;
        }
        shared_ptr<Employee> emp_to_find =  this->getEmployeesMultiStructures()->findEmployee(employeeID);
        if (emp_to_find == nullptr)
        {
            return FAILURE;
        }
        long long int company_id = emp_to_find->getEmployersid();
        Company* comp = this->companies_union.find(company_id)->data;
        if (emp_to_find->getEmployeeSalary() == 0)
        {
            this->employees.decreaseNumOfEmployeesWithNoSalary();
            this->employees.decreaseSumOfGradesEmployeesWithNoSalary(emp_to_find->getEmployeeGrade());
            comp->getEmployeesMultiStructures()->decreaseNumOfEmployeesWithNoSalary();
            comp->getEmployeesMultiStructures()->decreaseSumOfGradesEmployeesWithNoSalary(emp_to_find->getEmployeeGrade());
            emp_to_find->increaseSalary(salaryIncrease);
            this->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
            comp->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
            return SUCCESS;
        }
        else if (emp_to_find->getEmployeeSalary() > 0)
        {
            this->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->removeNode(emp_to_find->getEmployeeSalaryID()); // remove old node!
            comp->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->removeNode(emp_to_find->getEmployeeSalaryID()); // remove old node!
            emp_to_find->increaseSalary(salaryIncrease);
            this->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
            comp->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
        }
        
        return SUCCESS;
    }

    StatusType Industry::PromoteEmployee(int employeeID, int bumpGrade)
    {
        if (employeeID <= 0 )
        {
            return INVALID_INPUT;
        }
        //saleh
        shared_ptr<Employee> emp_to_find =  this->getEmployeesMultiStructures()->findEmployee(employeeID);
        if (emp_to_find == nullptr)
        {
            return FAILURE;
        }
        if(bumpGrade <= 0 ){ //CHECK THIS
            return SUCCESS;
        }
        long long int company_id = emp_to_find->getEmployersid();
        Company* comp = this->companies_union.find(company_id)->data;
        if (emp_to_find->getEmployeeSalary() == 0)
        {
            this->employees.decreaseSumOfGradesEmployeesWithNoSalary(emp_to_find->getEmployeeGrade());
            comp->getEmployeesMultiStructures()->decreaseSumOfGradesEmployeesWithNoSalary(emp_to_find->getEmployeeGrade());
            emp_to_find->bumpGrade(bumpGrade);
            this->employees.increaseSumOfGradesEmployeesWithNoSalary(emp_to_find->getEmployeeGrade());
            comp->getEmployeesMultiStructures()->increaseSumOfGradesEmployeesWithNoSalary(emp_to_find->getEmployeeGrade());
            return SUCCESS;
        }
        else if (emp_to_find->getEmployeeSalary() > 0)
        {
            this->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->removeNode(emp_to_find->getEmployeeSalaryID()); // remove old node!
            comp->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->removeNode(emp_to_find->getEmployeeSalaryID()); // remove old node!
            emp_to_find->bumpGrade(bumpGrade);
            this->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
            comp->getEmployeesMultiStructures()->getEmployeesWithSalaryTree()->addNode(emp_to_find->getEmployeeSalaryID(),emp_to_find,emp_to_find->getEmployeeGrade()); // im not sure if this is the correct implementation. ie, maybe we should pass salary or something else instead of level!
        }
        return SUCCESS;
    }

    StatusType Industry::SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m)
    {
        // complexity is O(log*(k) + log(n)) amortized on average
        if (companyID < 0 || companyID > this->getNumberOfCompanies() || m <= 0)
        {
            return INVALID_INPUT;
        }
        // company exists for sure!
        MultiStructures *multi =  (companyID == 0) ? this->getEmployeesMultiStructures() : companies_union.find(companyID)->data->getEmployeesMultiStructures(); // get the correct multistructure to work with!
        // now we need to find the employee whose rank is m!
        long long int sum = multi->SumOfBumpGradeBetweenTopWorkersByGroup(m);
        if ( sum < 0 )
        {
            return FAILURE;
        }
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", sum); // correct form???
        
        return SUCCESS;
    }

    StatusType Industry::AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary)
    {
        if (higherSalary < 0 || lowerSalary < 0 || (lowerSalary > higherSalary) || companyID < 0 || (companyID > this->getNumberOfCompanies()) )
        {
            return INVALID_INPUT;
        }
        if (companyID == 0){
            shared_ptr<Employee> fake_max = make_shared<Employee>(max_id+1,0,0,higherSalary);
            SalaryID max_sal_id = SalaryID(higherSalary, max_id+1);
            shared_ptr<Employee> fake_min = make_shared<Employee>(0,0,0,lowerSalary);
            SalaryID min_sal_id = SalaryID(lowerSalary, 0);

            //add fakes to tree
            AVL_Tree<SalaryID,shared_ptr<Employee>>* tree = employees.getEmployeesWithSalaryTree();
            tree->addNode(max_sal_id, fake_max, 0);
            tree->addNode(min_sal_id, fake_min, 0);

            long long int num_of_employees_in_bounds = 0;
            long double sum_of_grades = 0;

            int rank1 = tree->findRank(max_sal_id);
            int rank2 = tree->findRank(min_sal_id);

            int sum1 = tree->findSumSmaller(max_sal_id);
            int sum2 = tree->findSumSmaller(min_sal_id);

            num_of_employees_in_bounds = rank1 - rank2 -1;
            sum_of_grades = sum1 - sum2;

            if (lowerSalary == 0){
                num_of_employees_in_bounds += employees.getNumOfEmployeesWithNoSalary();
                sum_of_grades += employees.getSumOfGradesForEmployeesWithNoSalary();
            }
            tree->removeNode(max_sal_id); // potential bugs?
            tree->removeNode(min_sal_id);
            if (num_of_employees_in_bounds == 0){
                return FAILURE;
            }

            long double average = (sum_of_grades/num_of_employees_in_bounds);
            ////////////////////////// to remove!!!!!!!!!!!!!!!!
            // long long int temp = (long long int)( average * 10);
            // long double improved_average = ((long double)temp)/10;
            // if(abs(improved_average + 0.1 - average ) <= 0.0000000001){
            //     improved_average += 0.1;
            // }
            // average = improved_average;
            //////////////////////////// to remove!!!!!!!!!!!!!!
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1Lf\n", average);
            

            return SUCCESS;
        }
        
        Company* company = companies_union.find(companyID)->data;
        shared_ptr<Employee> fake_max = make_shared<Employee>(max_id+1,0,0,higherSalary);
        SalaryID max_sal_id = SalaryID(higherSalary, max_id+1);
        shared_ptr<Employee> fake_min = make_shared<Employee>(0,0,0,lowerSalary);
        SalaryID min_sal_id = SalaryID(lowerSalary, 0);

        //add fakes to tree
        AVL_Tree<SalaryID,shared_ptr<Employee>>* tree = company->getEmployeesMultiStructures()->getEmployeesWithSalaryTree();
        tree->addNode(max_sal_id, fake_max, 0);
        tree->addNode(min_sal_id, fake_min, 0);

        long long int num_of_employees_in_bounds = 0;
        long double sum_of_grades = 0;

        long long int rank1 = tree->findRank(max_sal_id);
        long long int rank2 = tree->findRank(min_sal_id);

        long long int sum1 = tree->findSumSmaller(max_sal_id);
        long long int sum2 = tree->findSumSmaller(min_sal_id);

        num_of_employees_in_bounds = rank1 - rank2 - 1 ;
        sum_of_grades = sum1 - sum2;

        if (lowerSalary == 0){
            num_of_employees_in_bounds += company->getEmployeesMultiStructures()->getNumOfEmployeesWithNoSalary();
            sum_of_grades += company->getEmployeesMultiStructures()->getSumOfGradesForEmployeesWithNoSalary();
        }
       tree->removeNode(max_sal_id);
       tree->removeNode(min_sal_id);
        if (num_of_employees_in_bounds == 0){
            return FAILURE;
        }
        tree->removeNode(max_sal_id);
        tree->removeNode(min_sal_id);
        long double average = (sum_of_grades/num_of_employees_in_bounds);

        ////////////////////////// to remove!!!!!!!!!!!!!!!!
            // long long int temp = (long long int)( average * 10);
            // long double improved_average = ((long double)temp)/10;
            // if(abs(improved_average + 0.1 - average ) <= 0.0000000001){
            //     improved_average += 0.1;
            // }
            // average = improved_average;
            //////////////////////////// to remove!!!!!!!!!!!!!!
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1Lf\n", average);
        return SUCCESS;
    }

    long double Industry::findSpecificCompanyValue(Company* comp,int companyID)
    {
        double owner_value = comp->getCompanyValue();
        double offset = companies_union.findOffSet(companyID);
        double real_value = owner_value - offset;
        return real_value;
    }

    StatusType Industry::CompanyValue(int companyID)
    {
        // complexity is O(log*(k)) amortized
        if ( companyID <= 0 || (companyID > this->getNumberOfCompanies()) )
        {
            return INVALID_INPUT;
        }

        Company* comp_to_find = companies_union.find(companyID)->data;
        // we know that the company has to exist because its ID is legal therefore there's no need to check if it was found.        
        long double owner_value = comp_to_find->getCompanyValue();
        long double offset = companies_union.findOffSet(companyID);
        long double real_value = owner_value - offset;
        printf("CompanyValue: %.1Lf\n", real_value);
        
        return SUCCESS; 
        
    }

    // StatusType Industry::BumpGradeToEmployees(int lowerSalary, int higherSalary, int bumpGrade)//bonus
    // {
    //     if ((lowerSalary > higherSalary) || (bumpGrade <= 0) )
    //     {
    //         return INVALID_INPUT;
    //     }
        


    //     return SUCCESS;
    // }

}