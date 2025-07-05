
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '@/components/ui/select';

interface MonthSelectorProps {
  label: string;
  availableMonths: string[];
  value: string;
  onChange: (value: string) => void;
  disabled?: boolean;
}

export const MonthSelector = ({ 
  label, 
  availableMonths, 
  value, 
  onChange, 
  disabled 
}: MonthSelectorProps) => {
  return (
    <div className="space-y-2">
      <label className="text-sm font-medium text-slate-300">{label}</label>
      <Select value={value} onValueChange={onChange} disabled={disabled}>
        <SelectTrigger className="bg-slate-700 border-slate-600 text-white">
          <SelectValue placeholder="Select month" />
        </SelectTrigger>
        <SelectContent className="bg-slate-700 border-slate-600">
          {availableMonths.map((month) => (
            <SelectItem 
              key={month} 
              value={month}
              className="text-white hover:bg-slate-600"
            >
              {month}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    </div>
  );
};
